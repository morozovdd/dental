#pragma once

#include "Vec3.hpp"

namespace dental::geometry {

struct Line3D {
  Vec3 point;
  Vec3 direction;

  Line3D() = delete;
  Line3D(Vec3 point, Vec3 direction);
};
} // namespace dental::geometry