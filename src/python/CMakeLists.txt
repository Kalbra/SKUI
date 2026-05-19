qt_add_library(python STATIC
    python_service.hpp python_service.cpp
    stdout_proxy.hpp stdout_proxy.cpp
)

target_link_libraries(python PRIVATE Qt6::Core pybind11::embed)
target_include_directories(python
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)