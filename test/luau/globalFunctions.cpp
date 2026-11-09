#include <gmock/gmock.h>
#include <gtest/gtest.h>
import luau;

void checkPrint(const luau::Environment& script) {
    testing::internal::CaptureStdout();
    luau::run::runLuau(*script, "print('asdf')");
    std::string out = testing::internal::GetCapturedStdout();

    EXPECT_THAT(out, testing::HasSubstr("asdf"));
}

TEST(Luau, GlobalFunctions) {
    luau::State main(luau::SecurityType::eDefault);
    luau::global::registerDefaultFunctions(*main);
    luau::sandbox(main);
    luau::Environment script(main);

    checkPrint(script);
}
