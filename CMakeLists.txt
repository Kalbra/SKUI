cmake_minimum_required(VERSION 4.2)
project(skui LANGUAGES CXX)

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

message("CCache executable: ${CCACHE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)


################ FIND PACKAGES AND ADD TARGETS ###############

find_package(Python REQUIRED COMPONENTS Interpreter Development)
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG QUIET)

if(NOT pybind11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        URL https://github.com/pybind/pybind11/archive/refs/tags/v3.0.4.tar.gz
    )
    FetchContent_MakeAvailable(pybind11)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets SerialPort)
qt_standard_project_setup()

add_subdirectory(src/app)
add_subdirectory(src/node)
add_subdirectory(src/nodeeditor)
add_subdirectory(src/panel)
add_subdirectory(src/python)

target_link_libraries(skui
    PRIVATE
        node
        nodeeditor
        panel
        python

        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)


################ MACRO DEFINITIONS ###############

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


################ INSTALLATION ###############

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
