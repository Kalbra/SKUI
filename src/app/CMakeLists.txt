qt_add_executable(skui 
    main.cpp
    skui.hpp skui.cpp
    mainwindow.hpp mainwindow.cpp mainwindow.ui
    context_menu.hpp context_menu.cpp
    rename_node_dialog.hpp rename_node_dialog.cpp
    propertywindow.hpp propertywindow.cpp
    document.hpp document.cpp
    ../../libs/icons.qrc
)

find_package(Git QUIET)
set(SKUI_BUILD_COMMIT "unknown")
set(SKUI_BUILD_TAG "")

if(GIT_FOUND)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse --short=8 HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE SKUI_BUILD_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --tags --abbrev=0 HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE SKUI_BUILD_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()

target_compile_definitions(skui PRIVATE
    SKUI_BUILD_COMMIT="${SKUI_BUILD_COMMIT}"
    SKUI_BUILD_TAG="${SKUI_BUILD_TAG}"
)

target_link_libraries(skui PRIVATE Qt6::Widgets)

set_target_properties(skui PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE ON
)