#include "commandregistry.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>

namespace evsh {

namespace {

    int cd(const CommandLine& commandLine)
    {
        if (commandLine.size() < 2) {
            std::cerr << "cd: No path specified" << std::endl;
            return EXIT_FAILURE;
        }
        const auto rc = chdir(commandLine[1].data());
        if (rc) {
            std::cerr << "cd: " << strerror(errno) << std::endl;
        }
        return rc;
    }

    int help(const CommandLine&)
    {
        std::cout << "Help for evsh internal commands\n\n"
                     "cd dir    - change current directory\n"
                     "help      - print this help\n"
                  << std::endl;
        return EXIT_SUCCESS;
    }
}

void CommandRegistry::add(const std::string_view name, Command command) {
    d_registry[std::string{name}] = command;
}

std::optional<int> CommandRegistry::tryExec(const CommandLine& commandLine) const
{
    const auto search = d_registry.find(commandLine[0]);
    if (search != d_registry.end()) {
        return search->second(commandLine);
    }
    return {};
}

CommandRegistry defaultCommandRegistry()
{
    CommandRegistry cr;
    cr.add("cd", cd);
    cr.add("help", help);

    return cr;
}
}
