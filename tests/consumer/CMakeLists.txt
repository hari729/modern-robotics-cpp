cmake_minimum_required(VERSION 3.20)
project(pkg_consumer LANGUAGES CXX)

find_package(pkg REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE pkg::pkg)
