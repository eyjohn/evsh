#include <string>
#include <string_view>
#include <vector>
namespace evsh {

struct CommandLine {
    std::string command;
    std::vector<std::string> arguments;
    bool operator==(const CommandLine&) const;
    explicit operator bool() const;
};

// Parse a text line into a CommandLine
CommandLine parse(std::string_view line);
}
