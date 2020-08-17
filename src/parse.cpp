#include "parse.h"
#include <bitset>
#include <iomanip>
#include <sstream>

using namespace std;

namespace evsh {
namespace {

};

void applyRedirects(CommandLine::Redirects& existing, const CommandLine::Redirects& next)
{
    for (auto i = 0; i < existing.size(); ++i) {
        if (!std::holds_alternative<CommandLine::EmptyRedirect>(next[i])) {
            existing[i] = next[i];
        }
    }
}

CommandLine parse(string_view line)
{
    CommandLine out;
    auto str = std::string {};
    auto iss = std::istringstream { line.data() };
    while (iss >> quoted(str)) {
        const auto redirects = tryParseRedirect(str);
        if (redirects.has_value()) {
            applyRedirects(out.redirects, *redirects);
        } else {
            out.arguments.push_back(std::move(str));
        }
    }
    return out;
}

std::optional<CommandLine::Redirects> tryParseRedirect(std::string_view argument)
{
    bitset<3> sources;
    int offset;
    bool append {};

    if (argument.find("<") == 0) {
        sources.set(0);
        offset = 1;
    } else if (argument.find(">") == 0) {
        sources.set(1);
        offset = 1;
    } else if (argument.find("1>") == 0) {
        sources.set(1);
        offset = 2;
    } else if (argument.find("2>") == 0) {
        sources.set(2);
        offset = 2;
    } else if (argument.find("&>") == 0) {
        sources.set(1);
        sources.set(2);
        offset = 2;
    } else {
        return {};
    }

    if (argument[offset] == '>') {
        append = true;
        ++offset;
    }

    CommandLine::Redirect destination;
    argument = argument.substr(offset);
    if (argument == "&1") {
        destination = CommandLine::RemapRedirect { 1 };
    } else if (argument == "&2") {
        destination = CommandLine::RemapRedirect { 2 };
    } else {
        destination = CommandLine::FileRedirect { string { argument }, append };
    }

    CommandLine::Redirects redirects;
    for (auto i = 0; i < sources.size(); ++i) {
        if (sources.test(i)) {
            redirects[i] = destination;
        }
    }
    return redirects;
}

}
