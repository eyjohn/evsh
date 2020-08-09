#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <variant>
#include <iostream>

using namespace ::testing;

TEST(TestMeh, InvalidCommand)
{
    std::variant<int, bool> foo;
    std::cout << foo.<int>() << std::endl;
}

