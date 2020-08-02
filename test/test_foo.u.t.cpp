#include "foo.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
TEST(TestFoo, Foo)
{
    EXPECT_THAT(evsh::foo(), Eq(42));
}
