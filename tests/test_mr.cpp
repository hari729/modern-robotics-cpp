#include "mr/mr.hpp"
#include <Eigen/Dense>
#include <gtest/gtest.h>

TEST(mr, skew) {
  Eigen::Vector3<double> v(1, 2, 3);
  Eigen::Matrix3d sv = mr::skew(v);
  Eigen::Matrix3d expected{{0, -3, 2}, {3, 0, -1}, {-2, 1, 0}};
  EXPECT_EQ(sv, expected);
}

TEST(mr, log3) {
  Eigen::Matrix3d R = Eigen::Matrix3d::Identity();

  // Call the function returning std::tuple
  auto [w, theta] = mr::log(R);

  // Expect theta to be 0
  EXPECT_NEAR(theta, 0.0, 1e-6);

  // Expect w to be the zero matrix
  EXPECT_TRUE(w.isApprox(Eigen::Vector3d::Zero(), 1e-6));
}

TEST(mr, getT) {
  Eigen::Matrix3d R{{0, -1, 0}, {0, 0, -1}, {1, 0, 0}};
  Eigen::Vector3d p(3, 0, 0);
  Eigen::Matrix4d T{{0, -1, 0, 3}, {0, 0, -1, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}};
  EXPECT_EQ(mr::getT(R, p), T);
}

TEST(mr, getTinv) {
  Eigen::Matrix4d T{{0, -1, 0, 3}, {0, 0, -1, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}};
  Eigen::Matrix4d Tinv{
      {0, 0, 1, 0}, {-1, 0, 0, 3}, {0, -1, 0, 0}, {0, 0, 0, 1}};
  EXPECT_EQ(mr::getTinv(T), Tinv);
}

TEST(mr, getAdjT) {
  Eigen::Matrix4d T{{0, 0, 1, 0}, {-1, 0, 0, 3}, {0, -1, 0, 0}, {0, 0, 0, 1}};
  Eigen::Matrix<double, 6, 6> AdjT{{0, 0, 1, 0, 0, 0},  {-1, 0, 0, 0, 0, 0},
                                   {0, -1, 0, 0, 0, 0}, {0, -3, 0, 0, 0, 1},
                                   {0, 0, 0, -1, 0, 0}, {0, 0, -3, 0, -1, 0}};
  EXPECT_EQ(mr::getAdjT(T), AdjT);
}

TEST(mr, log6) {
  Eigen::Matrix4d T{{0, -1, 0, 3}, {0, 0, -1, 0}, {1, 0, 0, 0}, {0, 0, 0, 1}};
  Eigen::Matrix<double, 6, 1> S{
      {0.5774, -0.5774, 0.5774, 1.0548, -1.0548, -0.6772}};
  auto [Scalc, theta] = mr::log(T);
  EXPECT_TRUE(Scalc.isApprox(S, 1e-4));
}

TEST(mr, twist_screw) {
  Eigen::Matrix<double, 6, 1> V{{0, 1, 2, 3, 0, 0}};
  double theta_expected = std::sqrt(5);
  Eigen::Matrix<double, 6, 1> S_expected = V / theta_expected;
  auto [S, theta] = mr::extractScrew(V);
  EXPECT_EQ(theta, theta_expected);
  EXPECT_EQ(S, S_expected);
}
