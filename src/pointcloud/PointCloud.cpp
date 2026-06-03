#include "dental/pointcloud/PointCloud.hpp"

namespace dental::pointcloud {

PointCloud::PointCloud(const std::vector<Eigen::Vector3d> &points)
    : points(points) {}

PointCloud::PointCloud(const dental::mesh::TriangleMesh &mesh) {
  for (const auto &face : mesh.faces) {
    // find centroid of each face and add it to the point cloud
    Eigen::Vector3d centroid =
        (mesh.vertices[face[0]] + mesh.vertices[face[1]] +
         mesh.vertices[face[2]]) /
        3.0;
    points.push_back(centroid);
  }
}
} // namespace dental::pointcloud
