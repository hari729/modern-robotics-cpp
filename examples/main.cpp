#include <iostream>

#include "mr/mr.hpp"

int main() {
  std::cout << mr::greet("CMake") << '\n';
  std::cout << "5! = " << mr::factorial(5) << '\n';
  return 0;
}
