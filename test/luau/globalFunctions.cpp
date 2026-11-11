#include <gmock/gmock.h>
#include <gtest/gtest.h>
import luau;
import luauHelper;

TEST(GlobalFunctions, Print) {
    luau::Environment script = luauHelper::getEnv(luau::SecurityType::eDefault);
    luau::global::registerDefaultFunctions(*script);

    testing::internal::CaptureStdout();
    luau::run::runLuau(*script, "print('asdf')");
    std::string out = testing::internal::GetCapturedStdout();

    EXPECT_THAT(out, testing::HasSubstr("asdf"));
}
