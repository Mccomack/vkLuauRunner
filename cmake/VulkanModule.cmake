find_package(Vulkan REQUIRED)

include_guard(GLOBAL)

add_library(VulkanModule)
add_library(Vulkan::cppm ALIAS VulkanModule)

target_compile_definitions(VulkanModule PUBLIC
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
    VULKAN_HPP_CPPM_MODULE
    VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
)
target_compile_options(VulkanModule PRIVATE -stdlib=libc++)
target_include_directories(VulkanModule PUBLIC "${Vulkan_INCLUDE_DIR}")
target_link_libraries(VulkanModule PUBLIC Vulkan::Vulkan)

set_target_properties(VulkanModule PROPERTIES
    CXX_STANDARD 23
    CXX_MODULE_STD ON          # import std 자동 처리
)

file(GLOB VULKAN_CPPM_FILES "${Vulkan_INCLUDE_DIR}/vulkan/*.cppm")

target_sources(VulkanModule PUBLIC
    FILE_SET CXX_MODULES
    BASE_DIRS "${Vulkan_INCLUDE_DIR}"
    FILES ${VULKAN_CPPM_FILES}
)