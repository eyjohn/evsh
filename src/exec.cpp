#include "exec.h"
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace evsh {
int exec(const CommandLine& commandLine)
{
    if (commandLine.empty()) {
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
        C_CommandLine c_cmdLine{ commandLine };
        const auto rc = execvp(c_cmdLine.data()[0], const_cast<char* const*>(c_cmdLine.data()));
        std::string error{ commandLine[0] };
        switch (errno) {
        case EACCES:
            error += ": permission denied";
            break;
        default:
            error += ": command not found";
            break;
        }
        std::cerr << error << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
}