include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0
  GIT_SHALLOW TRUE
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(mr_test test_mr.cpp)
target_link_libraries(mr_test PRIVATE mr::mr GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mr_test)
