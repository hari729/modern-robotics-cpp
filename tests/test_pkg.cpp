#include "pkg/pkg.hpp"

#include <gtest/gtest.h>

TEST(pkg, greet) {
  EXPECT_EQ(pkg::greet("world"), "Hello, world!");
}

TEST(pkg, factorial) {
  EXPECT_EQ(pkg::factorial(0), 1);
  EXPECT_EQ(pkg::factorial(1), 1);
  EXPECT_EQ(pkg::factorial(5), 120);
}
