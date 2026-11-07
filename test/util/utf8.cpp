#include <gtest/gtest.h>
import std;
import utf8;

TEST(Utf8, AsciiAdvancesOneByte) {
    std::size_t i = 0;

    EXPECT_EQ(utf8::index("abc", i), "a");
    EXPECT_EQ(i, 1u);
}

TEST(Utf8, TwoByteChar) {
    std::string s = "é";  // U+00E9
    std::size_t i = 0;

    EXPECT_EQ(utf8::index(s, i), s);
    EXPECT_EQ(i, 2u);
}

TEST(Utf8, ThreeByteChar) {
    std::string s = "한";
    std::size_t i = 0;

    EXPECT_EQ(utf8::index(s, i), s);
    EXPECT_EQ(i, 3u);
}

TEST(Utf8, FourByteChar) {
    std::string s = "😀";
    std::size_t i = 0;

    EXPECT_EQ(utf8::index(s, i), s);
    EXPECT_EQ(i, 4u);
}

TEST(Utf8, IteratesMixedString) {
    std::string s = "a한😀b";
    std::vector<std::string> out;

    for (std::size_t i = 0; i < s.size();)
        out.push_back(utf8::index(s, i));

    EXPECT_EQ(out, (std::vector<std::string>{"a", "한", "😀", "b"}));
}

TEST(Utf8, TruncatedSequenceDoesNotOverrun) {
    std::string s = "\xE4\xB8";  // 3-byte lead with only 2 bytes
    std::size_t i = 0;

    EXPECT_EQ(utf8::index(s, i), s);
}
