cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
set(CMAKE_CXX_MODULE_STD ON)

if(APPLE)
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

    set(CMAKE_EXE_LINKER_FLAGS "-L/opt/homebrew/opt/llvm/lib")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

    include_directories("/opt/homebrew/opt/llvm/include")

    set(CMAKE_CXX_STDLIB_MODULES_JSON "/opt/homebrew/opt/llvm/lib/c++/libc++.modules.json")

    execute_process(
        COMMAND xcrun --show-sdk-path
        OUTPUT_VARIABLE MACOS_SDK_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(CMAKE_OSX_SYSROOT ${MACOS_SDK_PATH})
endif()

set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(MyCpp3DGame VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

set(LUAU_BUILD_CLI OFF CACHE BOOL "" FORCE)
set(LUAU_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LUAU_BUILD_WEB OFF CACHE BOOL "" FORCE)

# git submodule 초기화
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
    endif()
endif()

# shader 컴파일
file(GLOB_RECURSE GLSL_FILES "src/shaders/*.vert" "src/shaders/*.frag" "src/shaders/*.comp")
file(GLOB_RECURSE SLANG_FILES "src/shaders/*.slang")

foreach(GLSL ${GLSL_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPV "${CMAKE_SOURCE_DIR}/build/shaders/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPV}
        COMMAND glslc ${GLSL} -o ${SPV}
        DEPENDS ${GLSL}
    )

    list(APPEND SPV_FILES ${SPV})
endforeach()

foreach(SLANG ${SLANG_FILES})
    get_filename_component(FILE_NAME ${SLANG} NAME)
    set(SPV "${CMAKE_SOURCE_DIR}/build/shaders/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPV}
        COMMAND slangc ${SLANG} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name -entry vertMain -entry fragMain -o ${SPV}
        DEPENDS ${SLANG}
    )

    list(APPEND SPV_FILES ${SPV})
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPV_FILES})

# 소스코드
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE MODULE_SOURCES "src/*.cppm" "src/*.ixx")

if(NOT SOURCES AND NOT MODULE_SOURCES)
    message(WARNING "No source files found in src/ directory")
endif()

add_executable(program ${SOURCES})

if(MODULE_SOURCES)
    target_sources(
        program PRIVATE FILE_SET CXX_MODULES FILES ${MODULE_SOURCES}
    )

    set_target_properties(program PROPERTIES CXX_SCAN_FOR_MODULES ON)

    # foreach(module_file ${MODULE_SOURCES})
    #     set_source_files_properties(${module_file} PROPERTIES
    #         COMPILE_FLAGS "-x c++-module"
    #     )
    # endforeach()
endif()

# git hash 정의
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions(program PRIVATE 
    GIT_HASH="${GIT_HASH}"
)

# version 정의
target_compile_definitions(program PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
    APP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    APP_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    APP_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

find_package(Vulkan REQUIRED)

# Vulkan hpp
add_library(VulkanHppModule)
add_library(Vulkan::cppm ALIAS VulkanHppModule)

target_compile_definitions(VulkanHppModule PUBLIC
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
    VULKAN_HPP_CPPM_MODULE
    VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
)
target_include_directories(VulkanHppModule PUBLIC "${Vulkan_INCLUDE_DIR}")
target_link_libraries(VulkanHppModule PUBLIC Vulkan::Vulkan)

set_target_properties(VulkanHppModule PROPERTIES
    CXX_STANDARD 23
    CXX_MODULE_STD ON          # import std 자동 처리
)

file(GLOB VULKAN_CPPM_FILES "${Vulkan_INCLUDE_DIR}/vulkan/*.cppm")

target_sources(VulkanHppModule PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS "${Vulkan_INCLUDE_DIR}"
    FILES ${VULKAN_CPPM_FILES}
)

add_subdirectory(external/glfw)
add_subdirectory(external/luau)

target_include_directories(program PRIVATE external/json/include)
target_include_directories(program PRIVATE external/glfw/include)
target_include_directories(program PRIVATE external/glm)
target_include_directories(program PRIVATE external/luau/VM/include)
target_include_directories(program PRIVATE external/luau/Compiler/include)
target_include_directories(program PRIVATE external/luau/Common/include)

target_include_directories(program PRIVATE ${CMAKE_SOURCE_DIR}/includes)

target_compile_features(program PRIVATE cxx_std_23)

target_link_libraries(program PRIVATE 
    Vulkan::Vulkan
    VulkanHppModule
    glfw

    Luau.VM
    Luau.Compiler
    # Luau.CodeGen
)

target_compile_options(program PRIVATE 
    # $<$<CXX_COMPILER_ID:GNU>:-fmodules-ts>
    # $<$<CXX_COMPILER_ID:Clang>:-fmodules>
    # $<$<CXX_COMPILER_ID:MSVC>:/experimental:module>

    $<$<AND:$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>,$<CONFIG:Debug>>:-g -O0>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Zi /Od>
)

target_compile_definitions(program PRIVATE 
    GLFW_INCLUDE_VULKAN
    VULKAN_HPP_CPPM_MODULE
)