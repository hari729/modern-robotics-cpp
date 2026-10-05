#include "mr/mr.hpp"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;

namespace mr {

Matrix3d skew(const Vector3<double> &v) { return v.asSkewSymmetric(); }
Matrix3d exp(const Matrix3d &w, const double &theta) {
  return Eigen::Matrix3d::Identity() + std::sin(theta) * w +
         (1 - std::cos(theta)) * w * w;
}
std::tuple<Vector3d, double> log(const Matrix3d &R) {
  double theta = std::acos((R.trace() - 1) / 2);
  Vector3d w;
  if (std::abs(theta) < 1e-6) {
    w = Vector3d::Zero();
  } else if (std::abs(R.trace() + 1) < 1e-6) {
    Vector3d temp(R(0, 0), R(1, 0), R(2, 0) + 1);
    w = temp / std::sqrt(2 * (1 + R(0, 0)));
  } else {
    Matrix3d w_skew = (R - R.transpose()) / (2 * std::sin(theta));
    w(0) = w_skew(2, 1);
    w(1) = w_skew(0, 2);
    w(2) = w_skew(1, 0);
  }
  return std::make_tuple(w, theta);
}
Matrix3d getRtp(const Matrix3d &R) { return R.transpose(); }
Matrix3d getRinv(const Matrix3d &R) { return R.transpose(); }
Matrix4d getT(const Matrix3d &R, const Vector3<double> &p) {
  Matrix4d T = Matrix4d::Identity();
  T.block<3, 3>(0, 0) = R;
  T.block<3, 1>(0, 3) = p;
  return T;
}
Matrix4d getTinv(const Matrix4d &T) {
  Matrix3d R = T.block<3, 3>(0, 0);
  Vector3<double> p = T.block<3, 1>(0, 3);
  Matrix4d Tinv = Matrix4d::Identity();
  Tinv.block<3, 3>(0, 0) = R.transpose();
  Tinv.block<3, 1>(0, 3) = -R.transpose() * p;
  return Tinv;
}
Matrix<double, 6, 6> getAdjT(const Matrix4d &T) {
  Matrix3d R = T.block<3, 3>(0, 0);
  Vector3<double> p = T.block<3, 1>(0, 3);
  Matrix<double, 6, 6> AdjT = Matrix<double, 6, 6>::Zero();
  AdjT.block<3, 3>(0, 0) = R;
  AdjT.block<3, 3>(3, 0) = skew(p) * R;
  AdjT.block<3, 3>(3, 3) = R;
  return AdjT;
}
Matrix4d exp(const Matrix<double, 6, 1> &S, const double &theta) {
  Vector3d Sw = S.block<3, 1>(0, 0);
  Matrix3d Sw_hat = skew(Sw);
  Vector3d Sv = S.block<3, 1>(3, 0);
  Matrix4d T = Matrix4d::Identity();
  T.block<3, 3>(0, 0) = exp(Sw_hat, theta);
  T.block<3, 1>(0, 3) =
      (Matrix3d::Identity() * theta + (1 - std::cos(theta)) * Sw_hat +
       (theta - std::sin(theta)) * (Sw_hat * Sw_hat)) *
      Sv;
  return T;
}
Matrix<double, 6, 1> getScrew(const Vector3d &s, const Vector3d &q,
                              const double &h) {
  Matrix<double, 6, 1> screw;
  screw.block<3, 1>(0, 0) = s;
  screw.block<3, 1>(3, 0) = -skew(s) * q + h * s;
  return screw;
}
std::tuple<Matrix<double, 6, 1>, double> log(const Matrix4d &T) {
  Matrix3d R = T.block<3, 3>(0, 0);
  Vector3<double> p = T.block<3, 1>(0, 3);
  Matrix<double, 6, 1> S;
  if (R.isIdentity(1e-6)) {
    S.block<3, 1>(0, 0) = Vector3d::Zero();
    S.block<3, 1>(3, 0) = p.normalized();
    return std::make_tuple(S, p.norm());
  }
  auto [w, theta] = log(R);
  Matrix3d ginv =
      (1 / theta) * Matrix3d::Identity() - 0.5 * skew(w) +
      ((1 / theta) - (1 / (2 * std::tan(theta / 2)))) * (skew(w) * skew(w));
  Vector3d v = ginv * p;
  S.block<3, 1>(0, 0) = w;
  S.block<3, 1>(3, 0) = v;
  return std::make_tuple(S, theta);
}
std::tuple<Matrix<double, 6, 1>, double>
extractScrew(const Matrix<double, 6, 1> &V) {
  Vector3d w = V.block<3, 1>(0, 0);
  Vector3d v = V.block<3, 1>(3, 0);
  Matrix<double, 6, 1> S;
  double theta;
  if (not w.isApprox(Vector3d::Zero(), 1e-6)) {
    theta = w.norm();
  } else {
    theta = v.norm();
  }
  S = V / theta;
  return std::make_tuple(S, theta);
}
} // namespace mr
