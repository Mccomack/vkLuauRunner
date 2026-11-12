#include <gtest/gtest.h>

import signal;

TEST(Signal, Logic) {
    Signal<int> sig;

    int didFired = 0;
    int in = 0;

    auto conn = sig.connect([&](const int& oin) {
        didFired++;
        in = oin;
    });

    sig.fire(42);

    EXPECT_EQ(didFired, 1);
    EXPECT_EQ(in, 42);
}

TEST(Signal, MultiConnection) {
    Signal sig;

    int didFired = 0;

    auto conn = sig.connect([&]() { didFired++; });

    int did1Fired = 0;

    auto conn1 = sig.connect([&]() { did1Fired++; });

    EXPECT_TRUE(didFired == did1Fired && didFired == 0);

    sig.fire();
    EXPECT_TRUE(didFired == did1Fired && didFired == 1);
}

TEST(Signal, Disconnect) {
    Signal sig;

    int didFired = 0;
    auto conn = sig.connect([&]() { didFired++; });

    int didFired1 = 0;

    auto conn1 = sig.connect([&]() { didFired1++; });

    sig.fire();

    conn.disconnect();

    sig.fire();

    EXPECT_EQ(didFired, 1);
    EXPECT_EQ(didFired1, 2);
}
