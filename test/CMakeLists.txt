include(GoogleTest)
include(${CMAKE_SOURCE_DIR}/cmake/VulkanModule.cmake)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")
file(GLOB_RECURSE SRC_MODULES "${CMAKE_SOURCE_DIR}/src/*.cppm")

if(NOT TEST_SOURCES)
    message(WARNING "No test files found in test/ directory")
    return()
endif()

add_executable(program_test ${TEST_SOURCES})

if(SRC_MODULES)
    target_sources(program_test PRIVATE 
        FILE_SET CXX_MODULES 
        BASE_DIRS ${CMAKE_SOURCE_DIR}/src
        FILES ${SRC_MODULES}
    )
    set_target_properties(program_test PROPERTIES CXX_SCAN_FOR_MODULES ON)
endif()

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions(program_test PRIVATE
    GIT_HASH="${GIT_HASH}"

    APP_VERSION="${PROJECT_VERSION}"
    APP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    APP_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    APP_VERSION_PATCH=${PROJECT_VERSION_PATCH}

    GLFW_INCLUDE_VULKAN
    VULKAN_HPP_CPPM_MODULE
)

target_include_directories(program_test PRIVATE
    ${CMAKE_SOURCE_DIR}/external/json/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/luau/VM/include
    ${CMAKE_SOURCE_DIR}/external/luau/Compiler/include
    ${CMAKE_SOURCE_DIR}/external/luau/CodeGen/include
    ${CMAKE_SOURCE_DIR}/external/luau/Common/include

    ${CMAKE_SOURCE_DIR}/includes
)

target_link_libraries(program_test PRIVATE
    GTest::gtest_main

    Vulkan::Vulkan
    VulkanModule

    glfw

    Luau.VM
    Luau.Compiler
    Luau.CodeGen
)

target_compile_options(program_test PRIVATE
    -stdlib=libc++
    $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-g -O0>
)

target_compile_definitions(program_test PRIVATE
    GIT_HASH="${GIT_HASH}"
)

gtest_discover_tests(program_test)