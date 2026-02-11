cmake_minimum_required(VERSION 3.28)
project(MyCpp3DGame VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.cppm"
    "src/*.ixx"
)

if(NOT SOURCES)
    message(WARNING "No source files found in src/ directory")
endif()

add_executable(program ${SOURCES})

target_compile_features(program PRIVATE cxx_std_23)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(program PRIVATE -fmodules-ts)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(program PRIVATE -fmodules)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(program PRIVATE /experimental:module)
endif()