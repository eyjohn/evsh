#include "commandline.h"
#include "commandregistry.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;
using namespace evsh;

TEST(TestCommandRegistry, AddAndTryExec)
{
    CommandRegistry cr;
    cr.add("test", [](auto) { return 42; });
    const auto res = cr.tryExec({{ "test" }});
    EXPECT_THAT(res, Optional(42));
}

TEST(TestCommandRegistry, TryExecMissing)
{
    CommandRegistry cr;
    const auto res = cr.tryExec({{ "test" }});
    EXPECT_THAT(res.has_value(), IsFalse());
}

TEST(TestCommandRegistry, defaultCommandRegistry)
{
    const auto cr = defaultCommandRegistry();
    auto res = cr.tryExec({{ "help" }});
    EXPECT_THAT(res, Optional(0));
}
