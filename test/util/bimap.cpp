#include <gtest/gtest.h>
import std;
import bimap;

TEST(Bimap, InsertAndLookupBothWays) {
    bimap<int, std::string> m;
    m.insert(1, "one");

    EXPECT_EQ(m.getB(1), "one");
    EXPECT_EQ(m.getA("one"), 1);
}

TEST(Bimap, MissingKeyThrows) {
    bimap<int, std::string> m;

    EXPECT_THROW(m.getB(1), std::out_of_range);
    EXPECT_THROW(m.getA("none"), std::out_of_range);
}

TEST(Bimap, RemoveByBothSides) {
    bimap<int, std::string> m;
    m.insert(1, "one");
    m.insert(2, "two");

    m.remove(1, "one");
    EXPECT_THROW(m.getB(1), std::out_of_range);
    EXPECT_THROW(m.getA("one"), std::out_of_range);

    EXPECT_EQ(m.getB(2), "two");
}

TEST(Bimap, RemoveByA) {
    bimap<int, std::string> m;
    m.insert(1, "one");

    m.remove(1);

    EXPECT_THROW(m.getB(1), std::out_of_range);
    EXPECT_THROW(m.getA("one"), std::out_of_range);
}

TEST(Bimap, RemoveByB) {
    bimap<int, std::string> m;
    m.insert(1, "one");

    m.remove(std::string("one"));

    EXPECT_THROW(m.getB(1), std::out_of_range);
    EXPECT_THROW(m.getA("one"), std::out_of_range);
}
