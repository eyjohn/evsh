#include "commandregistry.h"
#include "exec.h"
#include "parse.h"
#include <csignal>
#include <iostream>

using namespace evsh;

int main(int argc, char* argv[])
{
    std::signal(SIGINT, SIG_IGN);
    const auto commandRegistry = defaultCommandRegistry();
    for (;;) {
        std::string line;
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        const auto commandLine = parse(line);
        int rc;
        if (!commandLine.arguments.empty()) {
            const auto res = commandRegistry.tryExec(commandLine);
            if (res.has_value()) {
                rc = res.value();
            } else {
                rc = exec(commandLine);
            }
        }
    }
    return 0;
}
