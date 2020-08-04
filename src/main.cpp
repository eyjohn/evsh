#include "commandline.h"
#include "exec.h"
#include <csignal>
#include <iostream>

using namespace evsh;

int main(int argc, char* argv[])
{
    std::signal(SIGINT, SIG_IGN);
    for (;;) {
        std::string line;
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        const auto commandLine = parse(line);
        auto rc = exec(commandLine);
    }
    return 0;
}
