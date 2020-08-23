#include "parse.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <tuple>
#include <variant>

using namespace ::testing;
using namespace evsh;

MATCHER_P(IsRedirect, other, "")
{
    if (arg.index() != other.index()) {
        return false;
    }
    if (std::holds_alternative<CommandLine::EmptyRedirect>(arg)) {
        return true;
    } else if (std::holds_alternative<CommandLine::FileRedirect>(arg)) {
        return std::get<CommandLine::FileRedirect>(arg).filename == std::get<CommandLine::FileRedirect>(other).filename && std::get<CommandLine::FileRedirect>(arg).append == std::get<CommandLine::FileRedirect>(other).append;
    } else if (std::holds_alternative<CommandLine::RemapRedirect>(arg)) {
        return std::get<CommandLine::RemapRedirect>(arg).fd == std::get<CommandLine::RemapRedirect>(other).fd;
    }
    return false;
}

TEST(TestParse, ParseEmpty)
{
    EXPECT_THAT(parse("").arguments, Eq(CommandLine {}.arguments));
}

TEST(TestParse, ParseCommandOnly)
{
    EXPECT_THAT(parse("command").arguments, Eq(CommandLine { { "command" } }.arguments));
}

TEST(TestParse, ParseCommandWithOneArg)
{
    EXPECT_THAT(parse("command arg").arguments, Eq(CommandLine { { "command", "arg" } }.arguments));
}

TEST(TestParse, ParseCommandWithManyArgs)
{
    EXPECT_THAT(parse("command arg1 arg2").arguments, Eq(CommandLine { { "command", "arg1", "arg2" } }.arguments));
}

TEST(TestParse, ParseCommandWithQuotedArg)
{
    EXPECT_THAT(parse("command \"quoted arg\"").arguments, Eq(CommandLine { { "command", "quoted arg" } }.arguments));
}

TEST(TestParse, ParseCommandWithRedirects)
{
    const auto cmdLine = parse("command >file");
    EXPECT_THAT(cmdLine.arguments, Eq(CommandLine::Arguments { "command" }));
    EXPECT_THAT(cmdLine.redirects[1], IsRedirect(CommandLine::Redirect { CommandLine::FileRedirect { "file" } }));
}

TEST(TestApplyRedirects, ApplyIncremental)
{
    CommandLine::Redirects existing {
        CommandLine::EmptyRedirect {},
        CommandLine::FileRedirect { "2" },
        CommandLine::EmptyRedirect {}
    };
    const CommandLine::Redirects next {
        CommandLine::FileRedirect { "1" },
        CommandLine::EmptyRedirect {},
        CommandLine::FileRedirect { "3" }
    };
    applyRedirects(existing, next);
    EXPECT_THAT(existing[0], IsRedirect(CommandLine::Redirect { CommandLine::FileRedirect { "1" } }));
    EXPECT_THAT(existing[1], IsRedirect(CommandLine::Redirect { CommandLine::FileRedirect { "2" } }));
    EXPECT_THAT(existing[2], IsRedirect(CommandLine::Redirect { CommandLine::FileRedirect { "3" } }));
}

TEST(TestTryParseRedirect, NonRedirectCase)
{
    const auto redirects = tryParseRedirect("foo");
    EXPECT_THAT(redirects.has_value(), IsFalse());
}

struct RedirectParseTestParams {
    const char* param;
    CommandLine::Redirects redirects;
    const char* message;
};
std::ostream& operator<<(std::ostream& stream, const RedirectParseTestParams& obj)
{
    return stream << obj.message << ": " << obj.param;
}

class TestWithRedirectParseTestParams : public testing::TestWithParam<RedirectParseTestParams> {
};

TEST_P(TestWithRedirectParseTestParams, TryParseRedirect)
{
    const auto redirects = tryParseRedirect(GetParam().param);
    EXPECT_THAT(redirects.has_value(), IsTrue());
    for (auto i = 0; i < redirects->size(); ++i) {
        EXPECT_THAT((*redirects)[i], IsRedirect(GetParam().redirects[i])) << "Incorrect Redirect: " << i;
    }
}

INSTANTIATE_TEST_CASE_P(,
    TestWithRedirectParseTestParams,
    testing::Values(
        RedirectParseTestParams {
            ">file",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::FileRedirect { "file" },
                CommandLine::EmptyRedirect {} },
            "stdout to file" },
        RedirectParseTestParams {
            "2>file",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::EmptyRedirect {},
                CommandLine::FileRedirect { "file" } },
            "stderr to file" },
        RedirectParseTestParams {
            "&>file",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::FileRedirect { "file" },
                CommandLine::RemapRedirect { 1 } },
            "both stdout and stderr to file" },
        RedirectParseTestParams {
            ">>file",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::FileRedirect { "file", true },
                CommandLine::EmptyRedirect {} },
            "stdout append file" },
        RedirectParseTestParams {
            "2>>file",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::EmptyRedirect {},
                CommandLine::FileRedirect { "file", true } },
            "stderr append file" },
        RedirectParseTestParams {
            "&>>file",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::FileRedirect { "file", true },
                CommandLine::RemapRedirect { 1 } },
            "both stdout and stderr append file" },
        RedirectParseTestParams {
            "<file",
            CommandLine::Redirects {
                CommandLine::FileRedirect { "file" },
                CommandLine::EmptyRedirect {},
                CommandLine::EmptyRedirect {} },
            "stdin from file" },
        RedirectParseTestParams {
            "1>&2",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::RemapRedirect { 2 },
                CommandLine::EmptyRedirect {} },
            "stdout to stderr" },
        RedirectParseTestParams {
            "2>&1",
            CommandLine::Redirects {
                CommandLine::EmptyRedirect {},
                CommandLine::EmptyRedirect {},
                CommandLine::RemapRedirect { 1 } },
            "stderr to stdout" }));
