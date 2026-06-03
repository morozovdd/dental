#pragma once

#include "dental/mesh/TriangleMesh.hpp"
#include <Eigen/Core>
#include <vector>

namespace dental::pointcloud {

struct PointCloud {
  std::vector<Eigen::Vector3d> points;

  std::size_t pointCount() const { return points.size(); }
  bool empty() const { return points.empty(); }

  PointCloud() = default;
  PointCloud(const std::vector<Eigen::Vector3d> &points);
  PointCloud(const dental::mesh::TriangleMesh &mesh);
};
} // namespace dental::pointcloud
