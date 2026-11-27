cmake_minimum_required(VERSION 3.16)
project(SymulatorWypozyczalni VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql Svg)

qt_add_executable(SymulatorWypozyczalni
    src/main.cpp
    resources/resources.qrc
    src/ui/svg_helper.h
    src/ui/svg_helper.cpp
    src/ui/dashboard/dashboard.cpp
    src/ui/dashboard/dashboard.h
    src/ui/dashboard/dashboard.ui
    src/database/DatabaseManager.cpp
    src/database/DatabaseManager.h
    src/core/vehicles/Vehicle.cpp
    src/core/vehicles/Vehicle.h
    src/core/vehicles/PassengerCar.h
    src/core/vehicles/DeliveryVan.h
    src/core/vehicles/Scooter.h
    src/core/Rental.h
    src/core/Customer.h
    src/ui/dashboard/DashboardModels.h
    src/ui/dashboard/DashboardService.h
    src/ui/dashboard/DashboardService.cpp
    src/database/DatabaseSeeder.cpp
    src/database/DatabaseSeeder.h
)

target_include_directories(SymulatorWypozyczalni PRIVATE src/core src/services src/ui src/database)
qt_add_resources(SymulatorWypozyczalni "styles"
    PREFIX "/"
    BASE "src/ui/dashboard"
    FILES src/ui/dashboard/style.qss
)

target_include_directories(SymulatorWypozyczalni PRIVATE 
    src/core 
    src/services 
    src/ui
    src/ui/dashboard
)

target_link_libraries(SymulatorWypozyczalni PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Svg
)