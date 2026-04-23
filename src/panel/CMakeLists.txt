qt_add_library(panel STATIC
    resizeboundingbox.h resizeboundingbox.cpp
    panel.h panel.cpp
    visual_container.h visual_container.cpp
)

target_link_libraries(panel PRIVATE nodeeditor PRIVATE Qt6::Widgets)
target_include_directories(panel INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
