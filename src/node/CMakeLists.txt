qt_add_library(node STATIC
    node.hpp node.cpp
    interface.hpp interface.cpp
    nodes/visual.hpp nodes/visual.cpp
    nodes/label.hpp nodes/label.cpp
    nodes/slider.hpp nodes/slider.cpp
    nodes/serialsend.hpp nodes/serialsend.cpp
    nodes/lineedit.hpp nodes/lineedit.cpp
    nodes/textcombine.hpp nodes/textcombine.cpp
)

target_link_libraries(node PRIVATE Qt6::Widgets Qt6::SerialPort)
target_include_directories(node
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)