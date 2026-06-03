#pragma once

#include "dental/mesh/TriangleMesh.hpp"

#include <cstddef>
#include <vector>

#include <Eigen/Core>

namespace dental::mesh {

struct FaceFeatures {
  Eigen::Vector3d centroid;
  Eigen::Vector3d normal;
};

using FaceLabels = std::vector<int>;
using FaceAdjacency = std::vector<std::vector<std::size_t>>;

std::vector<FaceFeatures> computeFaceFeatures(const TriangleMesh &mesh);

FaceAdjacency computeFaceAdjacency(const TriangleMesh &mesh);

FaceLabels segmentByNormalRegionGrowing(const TriangleMesh &mesh,
                                        double max_normal_angle_degrees);

} // namespace dental::mesh