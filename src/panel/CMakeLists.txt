qt_add_library(panel STATIC
    resizeboundingbox.hpp resizeboundingbox.cpp
    panel.hpp panel.cpp
    visual_container.hpp visual_container.cpp
)

target_link_libraries(panel PRIVATE nodeeditor PRIVATE Qt6::Widgets)
target_include_directories(panel
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)
