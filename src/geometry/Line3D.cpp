#include "dental/geometry/Line3D.hpp"
#include "dental/geometry/Vec3.hpp"

namespace dental::geometry {

Line3D::Line3D(Vec3 point, Vec3 direction)
    : point(point), direction(direction.normalized()) {}
} // namespace dental::geometry