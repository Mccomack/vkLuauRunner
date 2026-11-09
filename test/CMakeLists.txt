include(GoogleTest)
include(${CMAKE_SOURCE_DIR}/cmake/VulkanModule.cmake)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
file(GLOB_RECURSE TEST_MODULES "*.cppm" "*.ixx")

if(NOT TEST_SOURCES AND NOT TEST_MODULES)
    message(WARNING "No test files found in test/ directory")
    return()
endif()

add_executable(program_test ${TEST_SOURCES})

add_subdirectory(
  ${CMAKE_SOURCE_DIR}/external/googletest
  ${CMAKE_BINARY_DIR}/external/googletest
)

target_link_libraries(program_test PRIVATE
    GTest::gtest_main
    GTest::gmock

    engine
)

gtest_discover_tests(program_test)
