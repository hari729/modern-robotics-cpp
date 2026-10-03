include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
  GIT_SHALLOW TRUE
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(pkg_test test_pkg.cpp)
target_link_libraries(pkg_test PRIVATE pkg::pkg GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(pkg_test)
