cmake_minimum_required(VERSION 3.19)
project(skui LANGUAGES CXX)

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

message("CCache executable: ${CCACHE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets SerialPort)
qt_standard_project_setup()

add_subdirectory(src/app)
add_subdirectory(src/node)
add_subdirectory(src/nodeeditor)
add_subdirectory(src/panel)

target_link_libraries(skui
    PRIVATE
        node
        nodeeditor
        panel

        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS skui
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET skui
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
