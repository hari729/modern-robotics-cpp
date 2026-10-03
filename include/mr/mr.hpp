#pragma once

#include <string>

namespace mr {

[[nodiscard]] auto greet(std::string const& name) -> std::string;

[[nodiscard]] constexpr auto factorial(int n) noexcept -> int {
  int result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= i;
  }
  return result;
}

}  // namespace mr
