#include "exec.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <optional>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <type_traits>
#include <unistd.h>
#include <variant>

namespace evsh {

namespace {

    bool configure_redirects(const CommandLine::Redirects& redirects)
    {
        int newFds[3] = { -1, -1, -1 };

        if (!std::visit([&newFds](auto&& redirect) {
                using T = std::decay_t<decltype(redirect)>;
                if constexpr (std::is_same_v<T, CommandLine::FileRedirect>) {
                    const auto fd = open(redirect.filename.c_str(), O_RDONLY);
                    if (fd == -1) {
                        std::cerr << redirect.filename << ": " << strerror(errno) << std::endl;
                        return false;
                    }
                    newFds[0] = fd;
                }
                return true;
            },
                redirects[0])) {
            return false;
        }

        for (auto i : { 1, 2 }) {
            if (!std::visit([&newFds, i](auto&& redirect) {
                    using T = std::decay_t<decltype(redirect)>;
                    if constexpr (std::is_same_v<T, CommandLine::FileRedirect>) {
                        const auto fd = open(redirect.filename.c_str(),
                            O_CREAT | O_WRONLY | (redirect.append ? O_APPEND : O_TRUNC),
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
                        if (fd == -1) {
                            std::cerr << redirect.filename << ": " << strerror(errno) << std::endl;
                            return false;
                        }
                        newFds[i] = fd;
                    } else if constexpr (std::is_same_v<T, CommandLine::RemapRedirect>) {
                        newFds[i] = redirect.fd;
                    }
                    return true;
                },
                    redirects[i])) {
                return false;
            }
        }

        for (auto i = 0; i < 3; ++i) {
            if (newFds[i] != -1) {
                if (dup2(newFds[i], i) == -1) {
                    std::cerr << strerror(errno) << std::endl;
                    return false;
                }
                if (newFds[i] > 2) { // only close hanging fds other than stdin/out/err
                    close(newFds[i]);
                }
            }
        }
        return true;
    }

}

int exec(const CommandLine& commandLine)
{
    if (commandLine.arguments.empty()) {
        return 0;
    }
    const auto pid = fork();
    if (pid) { // Parent
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus)) {
            return WEXITSTATUS(wstatus);
        }
        return -1;
    } else { // Child
        C_CommandLineArguments c_args { commandLine.arguments };
        if (!configure_redirects(commandLine.redirects)) {
            std::exit(EXIT_FAILURE);
        }
        execvp(c_args.data()[0], const_cast<char* const*>(c_args.data()));
        std::cerr << commandLine.arguments[0] << ": " << strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
}