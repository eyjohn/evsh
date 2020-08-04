#pragma once
#include "commandline.h"

namespace evsh {

// Execute the command specified in the `commandLine`, wait for it to complete, and return the status code.
// Return 0 if no command (empty commandLine) passed.
// Return -1 and log to stderr if failed to execute the command.
int exec(const CommandLine& commandLine);
}
