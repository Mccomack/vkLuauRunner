include(${CMAKE_SOURCE_DIR}/cmake/MacOSBundle.cmake)

set_source_files_properties(brewfix.cpp PROPERTIES
    CXX_SCAN_FOR_MODULES OFF
)

add_executable(program main.cpp)

target_link_libraries(program PRIVATE engine)

if(BUILD_BUNDLE)
    macos_bundle_app(program)
endif()
