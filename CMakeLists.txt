cmake_minimum_required(VERSION 3.29)

if(APPLE)
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

    set(CMAKE_EXE_LINKER_FLAGS "-L/opt/homebrew/opt/llvm/lib")

    include_directories("/opt/homebrew/opt/llvm/include")

    execute_process(
        COMMAND xcrun --show-sdk-path
        OUTPUT_VARIABLE MACOS_SDK_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CMAKE_OSX_SYSROOT ${MACOS_SDK_PATH})
endif()

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
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

add_custom_target(shaders ALL DEPENDS ${SPV_FILES})

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

    foreach(module_file ${MODULE_SOURCES})
        set_source_files_properties(${module_file} PROPERTIES
            COMPILE_FLAGS "-x c++-module"
        )
    endforeach()
endif()

find_package(Vulkan REQUIRED)

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

target_compile_definitions(program PRIVATE GLFW_INCLUDE_VULKAN)