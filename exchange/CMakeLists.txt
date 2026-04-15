cmake_minimum_required(VERSION 3.20)
project(easyIDS_exchange LANGUAGES CXX)

add_library(serial_wrapper
    src/serial_wrapper.cpp
)

target_include_directories(serial_wrapper PUBLIC include)

add_executable(easyIDSexchange src/main.cpp)

target_link_libraries(easyIDSexchange PRIVATE serial_wrapper)
