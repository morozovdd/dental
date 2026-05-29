#pragma once
#include <ostream>

namespace dental::geometry {

struct Vec3 {
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  Vec3() = default;
  Vec3(double x, double y, double z);

  Vec3 operator+(const Vec3 &other) const;
  Vec3 operator-(const Vec3 &other) const;
  Vec3 operator*(double scalar) const;

  double dot(const Vec3 &other) const;
  Vec3 cross(const Vec3 &other) const;
  double squaredNorm() const;
  double norm() const;
  Vec3 normalized() const;
};

std::ostream &operator<<(std::ostream &os, const Vec3 &v);
} // namespace dental::geometry