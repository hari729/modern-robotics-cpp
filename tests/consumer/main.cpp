#include <iostream>

#include "pkg/pkg.hpp"

int main() {
  std::cout << pkg::greet("consumer") << '\n';
  return 0;
}
