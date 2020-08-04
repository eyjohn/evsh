#include "commandline.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

namespace evsh {
namespace {

    const char* const* to_char_array(const CommandLine& input)
    {
        char const** const output = new char const* [input.size() + 1] { nullptr };
        std::transform(input.begin(), input.end(), output,
            [](const std::string& str) -> char const* { return str.c_str(); });
        return output;
    }
}

CommandLine parse(string_view line)
{
    CommandLine out;
    auto str = std::string{};
    auto iss = std::istringstream{ line.data() };
    while (iss >> quoted(str)) {
        out.push_back(std::move(str));
    }
    return out;
}

C_CommandLine::C_CommandLine(const CommandLine& commandLine)
    : d_data(to_char_array(commandLine))
{
}
C_CommandLine::~C_CommandLine()
{
    delete d_data;
}
const char* const* C_CommandLine::data() const
{
    return d_data;
}
}
