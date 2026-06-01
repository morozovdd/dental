#pragma once

#include "dental/mesh/TriangleMesh.hpp"
#include <Eigen/Dense>

namespace dental::mesh {

struct BoundingBox {
  Eigen::Vector3d min;
  Eigen::Vector3d max;
};

BoundingBox computeBoundingBox(const TriangleMesh &mesh);

Eigen::Vector3d computeCentroid(const TriangleMesh &mesh);

double computeSurfaceArea(const TriangleMesh &mesh);

double computeAverageEdgeLength(const TriangleMesh &mesh);

} // namespace dental::mesh