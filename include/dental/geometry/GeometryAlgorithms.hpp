#pragma once
#include "Line3D.hpp"
#include "Plane.hpp"
#include "Vec3.hpp"

namespace dental::geometry {

enum class IntersectionType { None, OnePoint, Infinite };

double distancePointPlane(const Vec3 &p, const Plane &plane);
double distancePointLine(const Vec3 &p, const Line3D &line);

IntersectionType intersectLinePlane(const Line3D &line, const Plane &plane,
                                    Vec3 &intersection);

IntersectionType intersectLines3D(const Line3D &l1, const Line3D &l2,
                                  Vec3 &intersection);
} // namespace dental::geometry