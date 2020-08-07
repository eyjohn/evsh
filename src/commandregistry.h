#pragma onec
#include "commandline.h"
#include <functional>
#include <map>
#include <string>
#include <string_view>

namespace evsh {

// A registry of internal commands which can be executed
class CommandRegistry {
public:
    // An executable command
    using Command = std::function<int(const CommandLine&)>;

    // Add a command to the registry
    void add(const std::string_view name, Command command);

    // Attempt to execute the command if present in the registry.
    // Returns the return code of the executed command if present.
    // Otherwise returns an empty optionable.
    std::optional<int> tryExec(const CommandLine& commandLine) const;

private:
    std::map<std::string, Command> d_registry;
};

// Instantiates a CommandRegistry and preload the built-in commands
CommandRegistry defaultCommandRegistry();
}
