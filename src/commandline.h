#pragma once
#include <string>
#include <string_view>
#include <vector>
namespace evsh {

using CommandLine = std::vector<std::string>;

// Temporary object that represents arguments
struct C_CommandLine {
    const char* const* data() const;
    C_CommandLine(const CommandLine& commandLine);
    ~C_CommandLine();

private:
    const char* const* d_data;
};

// Parse a text line into a CommandLine
CommandLine parse(std::string_view line);
} // namespace evsh
