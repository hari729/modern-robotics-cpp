#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;
namespace mr {
Eigen::Matrix3d skew(const Eigen::Vector3<double> &v);
Eigen::Matrix3d exp(const Eigen::Matrix3d &w, const double &theta);
std::tuple<Eigen::Vector3d, double> log(const Eigen::Matrix3d &R);
Eigen::Matrix3d getRinv(const Eigen::Matrix3d &R);
Eigen::Matrix3d getRtp(const Eigen::Matrix3d &R);
Eigen::Matrix4d getT(const Eigen::Matrix3d &R, const Eigen::Vector3<double> &p);
Eigen::Matrix4d getTinv(const Eigen::Matrix4d &T);
Matrix<double, 6, 6> getAdjT(const Matrix4d &T);
Matrix4d exp(const Matrix<double, 6, 1> &S, const double &theta);
Matrix<double, 6, 1> getScrew(const Vector3d &s, const Vector3d &q,
                              const double &h);
std::tuple<Matrix<double, 6, 1>, double> log(const Matrix4d &T);
std::tuple<Matrix<double, 6, 1>, double>
extractScrew(const Matrix<double, 6, 1> &V);
} // namespace mr
