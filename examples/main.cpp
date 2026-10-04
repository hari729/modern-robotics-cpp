#include "mr/mr.hpp"
#include <Eigen/Dense>
#include <iostream>

int main() {
  Eigen::Vector3<double> v;
  v << 1, 2, 3;
  Eigen::Matrix3d sv = mr::skew(v);
  std::cout << "v =" << std::endl << v << std::endl;
  std::cout << "skew v =" << std::endl << sv << std::endl;
  return 0;
}
