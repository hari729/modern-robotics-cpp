#include "pkg/pkg.hpp"

#include <sstream>

namespace pkg {

auto greet(std::string const& name) -> std::string {
  std::ostringstream oss;
  oss << "Hello, " << name << "!";
  return oss.str();
}

}  // namespace pkg
