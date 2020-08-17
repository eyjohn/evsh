#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <type_traits>
#include <variant>

using namespace ::testing;

TEST(TestMeh, Variant)
{
    // std::variant<int, bool> var;
    std::variant<int, bool> var;
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "int with value " << arg << '\n';
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << "bool with value " << arg << '\n';
        }
    },
        var);

    var = true;
    std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int>) {
            std::cout << "int with value " << arg << '\n';
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << "bool with value " << arg << '\n';
        }
    },
        var);
}
