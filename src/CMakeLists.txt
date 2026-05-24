# 외부 라이브러리 설정
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

set(LUAU_BUILD_CLI OFF CACHE BOOL "" FORCE)
set(LUAU_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LUAU_BUILD_WEB OFF CACHE BOOL "" FORCE)

# Vulkan hpp
find_package(Vulkan REQUIRED)

add_library(VulkanHppModule)
add_library(Vulkan::cppm ALIAS VulkanHppModule)

target_compile_definitions(VulkanHppModule PUBLIC
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
    VULKAN_HPP_CPPM_MODULE
    VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
)
target_compile_options(VulkanHppModule PRIVATE -stdlib=libc++)
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

# shader 컴파일
file(GLOB_RECURSE GLSL_FILES "shaders/*.vert" "shaders/*.frag" "shaders/*.comp")
file(GLOB_RECURSE SLANG_FILES "shaders/*.slang")

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
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE MODULE_SOURCES "*.cppm" "*.ixx")

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

set_source_files_properties(brewfix.cpp PROPERTIES
    CXX_SCAN_FOR_MODULES OFF
)

#  projectinfo 정의
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions(program PRIVATE 
    GIT_HASH="${GIT_HASH}"
)

target_compile_definitions(program PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
    APP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    APP_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    APP_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw external/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/luau external/luau)

target_include_directories(program PRIVATE 
    ${CMAKE_SOURCE_DIR}/external/json/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/luau/VM/include
    ${CMAKE_SOURCE_DIR}/external/luau/Compiler/include
    ${CMAKE_SOURCE_DIR}/external/luau/CodeGen/include
    ${CMAKE_SOURCE_DIR}/external/luau/Common/include

    ${CMAKE_SOURCE_DIR}/includes
)

target_compile_features(program PRIVATE cxx_std_23)

target_link_libraries(program PRIVATE 
    Vulkan::Vulkan
    VulkanHppModule

    glfw
    
    Luau.VM
    Luau.Compiler
    Luau.CodeGen
)

target_compile_options(program PRIVATE 
    -stdlib=libc++ 
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