qt_add_executable(skui 
    main.cpp
    skui.h skui.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
    context_menu.h context_menu.cpp
    rename_node_dialog.h rename_node_dialog.cpp
    propertywindow.h propertywindow.cpp
    document.h document.cpp
    ../../libs/icons.qrc
)

target_link_libraries(skui PRIVATE Qt6::Widgets)

set_target_properties(skui PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE ON
)