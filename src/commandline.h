#pragma once
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace evsh {

using CommandLine = std::vector<std::string>;

// struct CommandLine {
//     // Types
//     using Arguments = std::vector<std::string>;
//     struct FileRedirect {
//         std::string filename;
//     };
//     struct RemapRedirect {
//         int fd;
//     };
//     using Redirect = std::variant<FileRedirect, RemapRedirect>;
//     // using

//     // Data
//     Arguments arguments;
//     Redirect redirects[3];
// };


// std::variant<int, bool> foo;
// foo.
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
