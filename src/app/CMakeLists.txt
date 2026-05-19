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


target_include_directories(skui
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(skui PRIVATE Qt6::Widgets pybind11::embed)

set_target_properties(skui PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE ON
)