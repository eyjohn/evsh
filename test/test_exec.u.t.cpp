#include "commandline.h"
#include "exec.h"
#include "parse.h"
#include <cstdlib>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <unistd.h>

using namespace ::testing;
using namespace evsh;

TEST(TestExec, InvalidCommand)
{
    EXPECT_THAT(exec({ { "./nonexistent_command" } }), Eq(EXIT_FAILURE));
}

TEST(TestExec, ValidCommand)
{
    EXPECT_THAT(exec({ { "sh", "-c", "exit 42" } }), Eq(42));
}

TEST(TestExecRedirect, BasicToFileFromFileRedirectTest)
{
    exec(parse("echo return 42 >TestExecRedirect.BasicToFileFromFileRedirectTest"));
    EXPECT_THAT(exec(parse("sh <TestExecRedirect.BasicToFileFromFileRedirectTest")), Eq(42));
}

TEST(TestExecRedirect, RedirectBothOutToFile)
{
    exec(parse("sh -c \"echo 1; echo 2 1>&2\" &>TestExecRedirect.RedirectBothOutToFile"));
    EXPECT_THAT(exec(parse("sh -c \"return $(wc -l TestExecRedirect.RedirectBothOutToFile)\"")), Eq(2));
}
