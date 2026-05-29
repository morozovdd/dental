#pragma once
#include "Vec3.hpp"

namespace dental::geometry {

struct Plane {
  Vec3 point;
  Vec3 normal;

  Plane() = delete;
  Plane(Vec3 point, Vec3 normal);
  Plane(Vec3 p1, Vec3 p2, Vec3 p3);
};
} // namespace dental::geometry