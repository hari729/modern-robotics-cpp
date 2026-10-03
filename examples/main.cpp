#include <iostream>

#include "pkg/pkg.hpp"

int main() {
  std::cout << pkg::greet("CMake") << '\n';
  std::cout << "5! = " << pkg::factorial(5) << '\n';
  return 0;
}
