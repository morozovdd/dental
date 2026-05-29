#include "dental/geometry/Vec3.hpp"
#include "dental/geometry/MathUtils.hpp"
#include <cmath>
#include <stdexcept>

namespace dental::geometry {

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {};

Vec3 Vec3::operator+(const Vec3 &other) const {
  return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3 &other) const {
  return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(double scalar) const {
  return Vec3(x * scalar, y * scalar, z * scalar);
}

double Vec3::dot(const Vec3 &other) const {
  return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const {
  return Vec3(y * other.z - z * other.y, z * other.x - x * other.z,
              x * other.y - y * other.x);
}

double Vec3::squaredNorm() const { return x * x + y * y + z * z; }

double Vec3::norm() const { return std::sqrt(squaredNorm()); }

Vec3 Vec3::normalized() const {
  double n = norm();
  if (nearZero(n))
    throw std::invalid_argument("Cannot normalize a zero vector");
  return Vec3(x / n, y / n, z / n);
}

std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}
} // namespace dental::geometry