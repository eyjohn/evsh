#include "commandline.h"
#include <algorithm>

using namespace std;

namespace evsh {
namespace {

    const char* const* to_char_array(const CommandLine::Arguments& input)
    {
        char const** const output = new char const* [input.size() + 1] { nullptr };
        std::transform(input.begin(), input.end(), output,
            [](const std::string& str) -> char const* { return str.c_str(); });
        return output;
    }
}

C_CommandLineArguments::C_CommandLineArguments(const CommandLine::Arguments& arguments)
    : d_data(to_char_array(arguments))
{
}
C_CommandLineArguments::~C_CommandLineArguments()
{
    delete d_data;
}
const char* const* C_CommandLineArguments::data() const
{
    return d_data;
}
}
