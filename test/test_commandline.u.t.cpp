#include "commandline.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace evsh;

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
    EXPECT_THAT(parse("command arg"), Eq(CommandLine{ "command", "arg" }));
}

TEST(TestCommandLine, ParseCommandWithManyArgs)
{
    EXPECT_THAT(parse("command arg1 arg2"), Eq(CommandLine{ "command", "arg1", "arg2" }));
}

TEST(TestC_CommandLine, CommandOnly)
{
    const CommandLine cmdLine{ "command" };
    const C_CommandLine c_cmdLine{ cmdLine };
    EXPECT_THAT(c_cmdLine.data()[0], Eq(cmdLine[0].data()));
    EXPECT_THAT(c_cmdLine.data()[1], IsNull());
}

TEST(TestC_CommandLine, CommandWithArgs)
{
    const CommandLine cmdLine{ "command", { "arg" } };
    const C_CommandLine c_cmdLine{ cmdLine };
    EXPECT_THAT(c_cmdLine.data()[0], Eq(cmdLine[0].data()));
    EXPECT_THAT(c_cmdLine.data()[1], Eq(cmdLine[1].data()));
    EXPECT_THAT(c_cmdLine.data()[2], IsNull());
}