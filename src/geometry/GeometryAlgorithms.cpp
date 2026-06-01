#include "dental/geometry/GeometryAlgorithms.hpp"
#include "dental/geometry/MathUtils.hpp"
#include <cmath>

namespace dental::geometry {

double distancePointPlane(const Vec3 &p, const Plane &plane) {
  return std::abs((p - plane.point).dot(plane.normal));
}

double distancePointLine(const Vec3 &p, const Line3D &line) {
  Vec3 v = p - line.point;
  Vec3 projection = line.direction * v.dot(line.direction);
  return (v - projection).norm();
}

IntersectionType intersectLinePlane(const Line3D &line, const Plane &plane,
                                    Vec3 &intersection) {
  Vec3 d = line.direction;
  Vec3 n = plane.normal;
  double nd = n.dot(d);
  if (nearZero(nd)) {
    Vec3 v = plane.point - line.point;
    if (nearZero(v.dot(n)))
      return IntersectionType::Infinite;
    return IntersectionType::None;
  }
  Vec3 v = plane.point - line.point;
  double t = n.dot(v) / nd;
  intersection = line.point + d * t;
  return IntersectionType::OnePoint;
}

IntersectionType intersectLines3D(const Line3D &l1, const Line3D &l2,
                                  Vec3 &intersection) {
  // Lines are p1 + t*d1 and p2 + s*d2. If d1 x d2 is zero, the
  // directions are parallel, so there is no single intersection point.
  // Otherwise d1 x d2 is normal to both line directions. For two 3D lines
  // to intersect, p2 - p1 must lie in the same plane as d1 and d2, so its
  // dot product with that normal must be zero. Once that is true, solve for
  // t by crossing away the second line's parameter:
  //   t = ((p2 - p1) x d2) . (d1 x d2) / |d1 x d2|^2
  Vec3 p1 = l1.point;
  Vec3 d1 = l1.direction;
  Vec3 p2 = l2.point;
  Vec3 d2 = l2.direction;

  Vec3 n = d1.cross(d2);
  double denominator = n.dot(n);

  Vec3 p2MinusP1 = p2 - p1;

  if (nearZero(denominator)) {
    // Parallel. Check whether they are the same line.
    if (nearZero(p2MinusP1.cross(d1).norm())) {
      return IntersectionType::Infinite;
    }

    return IntersectionType::None;
  }

  // Non-parallel but not coplanar means skew.
  if (!nearZero(p2MinusP1.dot(n))) {
    return IntersectionType::None;
  }

  double t = p2MinusP1.cross(d2).dot(n) / denominator;
  intersection = p1 + d1 * t;

  return IntersectionType::OnePoint;
}
} // namespace dental::geometry