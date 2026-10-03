add_executable(pkg_example main.cpp)
target_link_libraries(pkg_example PRIVATE pkg::pkg)
