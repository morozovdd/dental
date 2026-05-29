#include "dental/geometry/Plane.hpp"

namespace dental::geometry {

Plane::Plane(Vec3 point, Vec3 normal)
    : point(point), normal(normal.normalized()) {}

Plane::Plane(Vec3 p1, Vec3 p2, Vec3 p3)
    : point(p1), normal((p2 - p1).cross(p3 - p1).normalized()) {}
} // namespace dental::geometry