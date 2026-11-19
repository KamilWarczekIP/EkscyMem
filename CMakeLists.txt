cmake_minimum_required(VERSION 3.19)
project(EkscyMem LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(EkscyMem
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
    backend.hpp backend.cpp
)

target_link_libraries(EkscyMem
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(EkscyMem PRIVATE Qt6::Core)

include(GNUInstallDirs)

# Visa dla keytheya
include_directories("C:\\Program Files\\IVI Foundation\\VISA\\Win64\\Include")
target_link_libraries(EkscyMem PRIVATE "C:\\Program Files\\IVI Foundation\\VISA\\Win64\\Lib_x64\\msc\\visa64.lib")


install(TARGETS EkscyMem
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET EkscyMem
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
