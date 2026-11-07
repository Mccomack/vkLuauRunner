#include <gmock/gmock.h>
#include <gtest/gtest.h>
import luau;

TEST(Luau, RunFromStr) {
    luau::State main(luau::SecurityType::eDefault);
    luau::global::registerDefaultFunctions(*main);
    luau::library::registerDefaultLibraries(*main);
    luau::object::registerDefaultObjects(*main);
    luau::sandbox(main);
    luau::Environment script(main);

    testing::internal::CaptureStdout();
    luau::run::runLuau(*script, "print('asdf')");
    std::string out = testing::internal::GetCapturedStdout();

    EXPECT_THAT(out, testing::HasSubstr("asdf"));
}
