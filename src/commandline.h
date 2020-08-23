#pragma once
#include <array>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace evsh {

struct CommandLine {
    // Types
    using Arguments = std::vector<std::string>;
    struct EmptyRedirect {
    };
    struct FileRedirect {
        std::string filename;
        bool append;
    };
    struct RemapRedirect {
        int fd;
    };
    using Redirect = std::variant<EmptyRedirect, FileRedirect, RemapRedirect>;
    using Redirects = std::array<Redirect, 3>;

    // Data
    Arguments arguments;
    Redirects redirects;
};

// Temporary object that represents arguments
struct C_CommandLineArguments {
    const char* const* data() const;
    C_CommandLineArguments(const CommandLine::Arguments& arguments);
    ~C_CommandLineArguments();

private:
    const char* const* d_data;
};

} // namespace evsh
