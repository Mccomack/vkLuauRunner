#include <gtest/gtest.h>

import std;
import defer;

TEST(Defer, Logic) {
    bool called = false;
    ([&]() { defer a([&]() { called = true; }); })();

    EXPECT_TRUE(called);
}

TEST(Defer, Cancel) {
    bool called = false;
    ([&]() {
        defer a([&]() { called = true; });

        a.Cancel();
    })();

    EXPECT_FALSE(called);
}

TEST(Defer, Order) {
    std::string order;
    {
        defer a([&]() { order += "a"; });
        defer b([&]() { order += "b"; });
    }

    EXPECT_EQ(order, "ba");
}
