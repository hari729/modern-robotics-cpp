cmake_minimum_required(VERSION 3.20)
project(mr_consumer LANGUAGES CXX)

find_package(mr REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE mr::mr)
