function(macos_bundle_app TARGET)
    if(NOT APPLE)
        return()
    endif()

    if(NOT DEFINED MACOSX_DEPLOYMENT_TARGET)
        set(MACOSX_DEPLOYMENT_TARGET "13.0")
    endif()

    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/Info.plist.in"
        MACOSX_BUNDLE_EXECUTABLE_NAME "${TARGET}"
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.mccomack.${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

    set(bundle_dir "$<TARGET_BUNDLE_DIR:${TARGET}>")
    set(resources_dir "$<TARGET_BUNDLE_CONTENT_DIR:${TARGET}>/Resources")

    # MoltenVK같은 경우에는 otool -L으로 뜨지 않기 때문에 따로 의존성 처리
    install(CODE "
        file(COPY \"${CMAKE_SOURCE_DIR}/asset\" DESTINATION \"${resources_dir}\")
        file(COPY \"${CMAKE_SOURCE_DIR}/build/shaders\" DESTINATION \"${resources_dir}\")
        include(BundleUtilities)
        fixup_bundle(\"${bundle_dir}\" \"\" \"\")
    ")

    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install "${CMAKE_BINARY_DIR}" --config $<CONFIG>
        COMMENT "Assembling ${TARGET}.app"
    )
endfunction()
