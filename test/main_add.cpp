#include <gtest/gtest.h>
import learn;

TEST(LearnTest, ADD) {
    EXPECT_EQ(test_learn_add(1, 2), 3);
}