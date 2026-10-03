#include "mr/mr.hpp"

#include <sstream>

namespace mr {

auto greet(std::string const& name) -> std::string {
  std::ostringstream oss;
  oss << "Hello, " << name << "!";
  return oss.str();
}

}  // namespace mr
