#include "dental/geometry/GeometryAlgorithms.hpp"
#include "dental/geometry/Line3D.hpp"
#include "dental/geometry/Plane.hpp"
#include "dental/geometry/Vec3.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dental::geometry;

namespace {
constexpr double kEpsilon = 1e-9;

bool near(double actual, double expected) {
  return std::abs(actual - expected) < kEpsilon;
}

void require(bool condition, const std::string &message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    std::exit(EXIT_FAILURE);
  }
}

void requireNear(double actual, double expected, const std::string &message) {
  if (!near(actual, expected)) {
    std::cerr << "FAIL: " << message << " expected " << expected << " got "
              << actual << '\n';
    std::exit(EXIT_FAILURE);
  }
}

void requireVecNear(const Vec3 &actual, const Vec3 &expected,
                    const std::string &message) {
  requireNear(actual.x, expected.x, message + " x");
  requireNear(actual.y, expected.y, message + " y");
  requireNear(actual.z, expected.z, message + " z");
}

void testVec3() {
  Vec3 a{1.0, 2.0, 3.0};
  Vec3 b{4.0, -2.0, 0.5};

  requireVecNear(a + b, Vec3{5.0, 0.0, 3.5}, "Vec3 addition");
  requireVecNear(a - b, Vec3{-3.0, 4.0, 2.5}, "Vec3 subtraction");
  requireVecNear(a * 2.0, Vec3{2.0, 4.0, 6.0}, "Vec3 scalar multiply");
  requireNear(a.dot(b), 1.5, "Vec3 dot product");
  requireVecNear(Vec3{1.0, 0.0, 0.0}.cross(Vec3{0.0, 1.0, 0.0}),
                 Vec3{0.0, 0.0, 1.0}, "Vec3 cross product");
  requireNear(Vec3{3.0, 4.0, 12.0}.norm(), 13.0, "Vec3 norm");

  Vec3 normalized = Vec3{0.0, 3.0, 4.0}.normalized();
  requireVecNear(normalized, Vec3{0.0, 0.6, 0.8}, "Vec3 normalized");
  requireNear(normalized.norm(), 1.0, "normalized vector has unit length");

  bool threw = false;
  try {
    Vec3{0.0, 0.0, 0.0}.normalized();
  } catch (const std::invalid_argument &) {
    threw = true;
  }
  require(threw, "normalizing zero vector throws");
}

void testLineAndPlaneConstruction() {
  Line3D line{Vec3{0.0, 0.0, 0.0}, Vec3{10.0, 0.0, 0.0}};
  requireVecNear(line.direction, Vec3{1.0, 0.0, 0.0},
                 "Line3D normalizes direction");

  Plane plane{Vec3{0.0, 0.0, 0.0}, Vec3{0.0, 0.0, 5.0}};
  requireVecNear(plane.normal, Vec3{0.0, 0.0, 1.0}, "Plane normalizes normal");
}

void testDistances() {
  Plane xyPlane{Vec3{0.0, 0.0, 0.0}, Vec3{0.0, 0.0, 2.0}};
  requireNear(distancePointPlane(Vec3{3.0, -1.0, 5.0}, xyPlane), 5.0,
              "point-plane distance");

  Line3D xAxis{Vec3{0.0, 0.0, 0.0}, Vec3{10.0, 0.0, 0.0}};
  requireNear(distancePointLine(Vec3{3.0, 4.0, 12.0}, xAxis), std::sqrt(160.0),
              "point-line distance");
}

void testIntersections() {
  Line3D zLine{Vec3{0.0, 0.0, 0.0}, Vec3{0.0, 0.0, 2.0}};
  Plane zFive{Vec3{0.0, 0.0, 5.0}, Vec3{0.0, 0.0, 1.0}};
  Vec3 intersection;

  require(intersectLinePlane(zLine, zFive, intersection) ==
              IntersectionType::OnePoint,
          "line intersects plane");
  requireVecNear(intersection, Vec3{0.0, 0.0, 5.0},
                 "line-plane intersection point");

  Line3D parallelLine{Vec3{0.0, 0.0, 1.0}, Vec3{1.0, 0.0, 0.0}};
  require(intersectLinePlane(parallelLine, zFive, intersection) ==
              IntersectionType::None,
          "parallel line does not intersect plane");

  Line3D xAxis{Vec3{0.0, 0.0, 0.0}, Vec3{1.0, 0.0, 0.0}};
  Line3D yAxis{Vec3{0.0, 0.0, 0.0}, Vec3{0.0, 1.0, 0.0}};
  require(intersectLines3D(xAxis, yAxis, intersection) ==
              IntersectionType::OnePoint,
          "perpendicular lines intersect");
  requireVecNear(intersection, Vec3{0.0, 0.0, 0.0},
                 "line-line intersection point");

  Line3D skewLine{Vec3{0.0, 0.0, 1.0}, Vec3{0.0, 1.0, 0.0}};
  require(intersectLines3D(xAxis, skewLine, intersection) ==
              IntersectionType::None,
          "skew lines do not intersect");
}
} // namespace

int main() {
  testVec3();
  testLineAndPlaneConstruction();
  testDistances();
  testIntersections();

  std::cout << "All geometry tests passed\n";
  return EXIT_SUCCESS;
}
