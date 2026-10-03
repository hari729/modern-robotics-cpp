#include "mr/mr.hpp"

#include <gtest/gtest.h>

TEST(mr, greet) {
  EXPECT_EQ(mr::greet("world"), "Hello, world!");
}

TEST(mr, factorial) {
  EXPECT_EQ(mr::factorial(0), 1);
  EXPECT_EQ(mr::factorial(1), 1);
  EXPECT_EQ(mr::factorial(5), 120);
}
