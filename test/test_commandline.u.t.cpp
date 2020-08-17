#include "commandline.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace evsh;

TEST(Test_C_CommandLineArguments, CommandOnly)
{
    const CommandLine cmdLine { { "command" } };
    const C_CommandLineArguments c_args { cmdLine.arguments };
    EXPECT_THAT(c_args.data()[0], Eq(cmdLine.arguments[0].data()));
    EXPECT_THAT(c_args.data()[1], IsNull());
}

TEST(Test_C_CommandLineArguments, CommandWithArgs)
{
    const CommandLine cmdLine { { "command", "arg" } };
    const C_CommandLineArguments c_args { cmdLine.arguments };
    EXPECT_THAT(c_args.data()[0], Eq(cmdLine.arguments[0].data()));
    EXPECT_THAT(c_args.data()[1], Eq(cmdLine.arguments[1].data()));
    EXPECT_THAT(c_args.data()[2], IsNull());
}
