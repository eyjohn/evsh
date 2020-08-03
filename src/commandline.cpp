#include "commandline.h"
#include <sstream>

using namespace std;

namespace evsh {

CommandLine parse(string_view line)
{
    CommandLine out;
    auto str = std::string{};
    auto iss = std::istringstream{ line.data() };
    while (iss >> str) {
        if (out.command.empty()) {
            out.command = str;
        } else {
            out.arguments.push_back(std::move(str));
        }
    }
    return out;
}
bool CommandLine::operator==(const CommandLine& other) const
{
    return command == other.command && arguments == other.arguments;
}
CommandLine::operator bool() const
{
    return !command.empty();
}
}
