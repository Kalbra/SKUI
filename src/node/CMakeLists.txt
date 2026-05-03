qt_add_library(node STATIC
    node.h node.cpp
    interface.h interface.cpp
    nodes/visual.h nodes/visual.cpp
    nodes/label.h nodes/label.cpp
    nodes/slider.h nodes/slider.cpp
    nodes/serialsend.h nodes/serialsend.cpp
    nodes/lineedit.h nodes/lineedit.cpp
    nodes/textcombine.h nodes/textcombine.cpp
)

target_link_libraries(node PRIVATE Qt6::Widgets Qt6::SerialPort)
target_include_directories(node INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})