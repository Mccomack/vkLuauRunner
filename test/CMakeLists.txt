include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
file(GLOB_RECURSE TEST_MODULES "*.cppm" "*.ixx")

if(NOT TEST_SOURCES AND NOT TEST_MODULES)
    message(WARNING "No test files found in test/ directory")
    return()
endif()

add_subdirectory(
  ${CMAKE_SOURCE_DIR}/external/googletest
  ${CMAKE_BINARY_DIR}/external/googletest
)

add_executable(program_test ${TEST_SOURCES})

if(TEST_MODULES)
    target_sources(program_test PRIVATE
        FILE_SET CXX_MODULES FILES ${TEST_MODULES}
    )
endif()

target_link_libraries(program_test PRIVATE
    engine
    GTest::gtest_main
    GTest::gmock
)

gtest_discover_tests(program_test)
