cmake_minimum_required(VERSION 3.16)
project(SymulatorWypozyczalni VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_add_executable(SymulatorWypozyczalni
    src/main.cpp
    src/ui/dashboard/dashboard.cpp
    src/ui/dashboard/dashboard.h
    src/ui/dashboard/dashboard.ui
)

target_include_directories(SymulatorWypozyczalni PRIVATE src/core src/services src/ui)

target_link_libraries(SymulatorWypozyczalni PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)