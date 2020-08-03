#include "commandline.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace evsh;

TEST(TestCommandLine, BoolFalse)
{
    EXPECT_THAT(CommandLine{}, IsFalse());
}

TEST(TestCommandLine, BoolTrue)
{
    EXPECT_THAT(CommandLine{ "command" }, IsTrue());
}

TEST(TestCommandLine, ParseEmpty)
{
    EXPECT_THAT(parse(""), Eq(CommandLine{}));
}

TEST(TestCommandLine, ParseCommandOnly)
{
    EXPECT_THAT(parse("command"), Eq(CommandLine{ "command" }));
}

TEST(TestCommandLine, ParseCommandWithOneArg)
{
    EXPECT_THAT(parse("command arg"), Eq(CommandLine{ "command", { "arg" } }));
}

TEST(TestCommandLine, ParseCommandWithManyArgs)
{
    EXPECT_THAT(parse("command arg1 arg2"), Eq(CommandLine{ "command", { "arg1", "arg2" } }));
}
