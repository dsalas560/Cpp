cmake_minimum_required(VERSION 3.15)

project(Parking_Garage VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Parking_Garage
    src/main.cpp
    src/core/Vehicle.cpp
    src/core/Ticket.cpp
    src/core/Garage.cpp
    src/core/Rate.cpp
)
