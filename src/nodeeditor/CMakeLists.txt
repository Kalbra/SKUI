qt_add_library(nodeeditor STATIC
    nodeeditor.hpp nodeeditor.cpp
    nodeeditor_scene.hpp nodeeditor_scene.cpp
    graphicsitems/node_graphicsitem.hpp graphicsitems/node_graphicsitem.cpp
    graphicsitems/pad.hpp graphicsitems/pad.cpp
    graphicsitems/cable.hpp graphicsitems/cable.cpp
)

target_link_libraries(nodeeditor PRIVATE Qt6::Widgets)
target_include_directories(nodeeditor INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})