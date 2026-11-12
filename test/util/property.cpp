#include <gtest/gtest.h>

import property;

TEST(Property, NoModification) {
    Property<bool> asdf = false;
    EXPECT_FALSE(asdf);

    asdf = true;
    EXPECT_TRUE(asdf);
}

TEST(Property, Comparison) {
    Property<int> val1 = 5;
    Property<int> val2 = 5;
    Property<int> val3 = 6;

    EXPECT_TRUE(val1 == val2);
    EXPECT_FALSE(val1 != val2);

    EXPECT_TRUE(val1 <= val3);
    EXPECT_FALSE(val3 <= val1);
}

TEST(Property, BindSet) {
    Property<bool> val = false;
    int calls = 0;

    bool didBinded = false;

    val.bindSet([&](const bool& old, const bool& req) {
        calls++;
        didBinded = true;

        return false;
    });

    val = true;

    EXPECT_FALSE(val);
    EXPECT_TRUE(didBinded);
    EXPECT_EQ(calls, 1);
}

TEST(Property, BindGet) {
    Property<bool> val = false;
    int calls = 0;

    bool didBinded = false;

    val.bindGet([&](const bool& now) {
        calls++;
        didBinded = true;

        return true;
    });

    EXPECT_TRUE(val);
    EXPECT_TRUE(didBinded);
    EXPECT_EQ(calls, 1);
}

TEST(Property, BindSetArgs) {
    Property<int> val = 1;

    int b_old, b_req;

    val.bindSet([&](const int& old, const int& req) {
        b_old = old;
        b_req = req;

        return req;
    });

    val = 2;

    EXPECT_EQ(b_old, 1);
    EXPECT_EQ(b_req, 2);
}

TEST(Property, BindGetArgs) {
    Property<int> val = 1;

    int b_now;

    val.bindGet([&](const int& now) {
        b_now = now;

        return now;
    });

    int _ = val;

    EXPECT_EQ(b_now, 1);
}
