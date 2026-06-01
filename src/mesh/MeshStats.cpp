#include "dental/mesh/MeshStats.hpp"
#include <limits>

namespace dental::mesh {

BoundingBox computeBoundingBox(const TriangleMesh &mesh) {
  BoundingBox bbox;
  bbox.min = Eigen::Vector3d::Constant(std::numeric_limits<double>::max());
  bbox.max = Eigen::Vector3d::Constant(std::numeric_limits<double>::lowest());

  for (const auto &vertex : mesh.vertices) {
    bbox.min = bbox.min.cwiseMin(vertex);
    bbox.max = bbox.max.cwiseMax(vertex);
  }

  return bbox;
}

Eigen::Vector3d computeCentroid(const TriangleMesh &mesh) {
  Eigen::Vector3d centroid = Eigen::Vector3d::Zero();

  for (const auto &vertex : mesh.vertices) {
    centroid += vertex;
  }

  centroid /= static_cast<double>(mesh.vertexCount());

  return centroid;
}

double computeSurfaceArea(const TriangleMesh &mesh) {
  double area = 0.0;

  for (const auto &face : mesh.faces) {
    const Eigen::Vector3d &v1 = mesh.vertices[face[0]];
    const Eigen::Vector3d &v2 = mesh.vertices[face[1]];
    const Eigen::Vector3d &v3 = mesh.vertices[face[2]];
    area += (v1 - v2).cross(v1 - v3).norm();
  }
  area /= 2.0;
  return area;
}

double computeAverageEdgeLength(const TriangleMesh &mesh) {
  double total_length = 0.0;

  for (const auto &face : mesh.faces) {
    const Eigen::Vector3d &v1 = mesh.vertices[face[0]];
    const Eigen::Vector3d &v2 = mesh.vertices[face[1]];
    const Eigen::Vector3d &v3 = mesh.vertices[face[2]];

    total_length += (v1 - v2).norm();
    total_length += (v2 - v3).norm();
    total_length += (v3 - v1).norm();
  }
  return total_length / (3 * mesh.faceCount());
}

} // namespace dental::mesh