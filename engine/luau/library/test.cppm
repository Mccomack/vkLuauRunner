export module luau:library.test;
import :common;

import utf8;

import std;

namespace {
    std::unordered_set<std::string> specialChars =
        {".", ",", "!", "?", "(", ")", "'", "\"", ";", ":", " "};
}

namespace luau::library {
    export class test;
}

class luau::library::test {
   private:
    test() = default;
    ~test() = default;

    test(const test&) = delete;
    test& operator=(const test&) = delete;

   public:
    static test& getInstance();

    std::string toMoneyFace(std::string_view original);
};

luau::library::test& luau::library::test::getInstance() {
    static test instance;

    return instance;
}

std::string luau::library::test::toMoneyFace(std::string_view original) {
    std::string moneyface = "";

    for (std::size_t i = 0; i < original.size();) {
        std::string str = utf8::index(original, i);

        if (specialChars.contains(str))
            moneyface += str;
        else
            moneyface += "🤑";
    }

    return moneyface;
}