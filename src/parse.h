#pragma once
#include "commandline.h"

#include <string_view>
#include <optional>

namespace evsh {

// Incrementally apply redirects
void applyRedirects(CommandLine::Redirects& existing, const CommandLine::Redirects& next);

// Parse a text line into a CommandLine
CommandLine parse(std::string_view line);

// Try parse a single argument into a redirect statement
std::optional<CommandLine::Redirects> tryParseRedirect(std::string_view argument);


} // namespace evsh
