#include "commandline.h"
#include "exec.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace ::testing;
using namespace evsh;

TEST(TestExec, InvalidCommand)
{
    EXPECT_THAT(exec({"./nonexistent_command"}), Eq(EXIT_FAILURE));
}

TEST(TestExec, ValidCommand)
{
    EXPECT_THAT(exec({"sh", "-c", "exit 42"}), Eq(42));
    EXPECT_TRUE(false) << "yup fail";
}
