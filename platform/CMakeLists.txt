if(APPLE)
    add_subdirectory(macos)
elseif(WIN32)
    add_subdirectory(windows)
else()
    add_subdirectory(linux)
endif()

set_target_properties(program PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
