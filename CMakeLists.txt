cmake_minimum_required(VERSION 3.28)
project(MyCpp3DGame VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

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

find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)

include_directories(external/json/include)
include_directories(external/glfw/include)
include_directories(external/glm)

include_directories(${CMAKE_SOURCE_DIR}/includes)

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

target_link_libraries(program 
    PRIVATE
    Vulkan::Vulkan
    glfw
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(program PRIVATE -fmodules-ts)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(program PRIVATE -fmodules)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(program PRIVATE /experimental:module)
endif()