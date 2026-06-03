
#include "dental/mesh/MeshSegmentation.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace dental::mesh {

namespace {

using Edge = std::pair<std::uint32_t, std::uint32_t>;

struct EdgeHash {
  std::size_t operator()(const Edge &edge) const {
    const std::size_t a = static_cast<std::size_t>(edge.first);
    const std::size_t b = static_cast<std::size_t>(edge.second);
    return (a << 32U) ^ b;
  }
};

Edge makeUndirectedEdge(std::uint32_t a, std::uint32_t b) {
  return {std::min(a, b), std::max(a, b)};
}

} // namespace

std::vector<FaceFeatures> computeFaceFeatures(const TriangleMesh &mesh) {
  std::vector<FaceFeatures> features;
  features.reserve(mesh.faceCount());

  for (const auto &face : mesh.faces) {
    const Eigen::Vector3d v1 = mesh.vertices[face[0]];
    const Eigen::Vector3d v2 = mesh.vertices[face[1]];
    const Eigen::Vector3d v3 = mesh.vertices[face[2]];

    Eigen::Vector3d centroid = (v1 + v2 + v3) / 3.0;

    Eigen::Vector3d normal = (v2 - v1).cross(v3 - v1);
    if (normal.norm() > 1e-12) {
      normal.normalize();
    }

    features.push_back({centroid, normal});
  }

  return features;
}

FaceAdjacency computeFaceAdjacency(const TriangleMesh &mesh) {
  FaceAdjacency adjacency(mesh.faceCount());
  std::unordered_map<Edge, std::size_t, EdgeHash> edge_to_face;

  for (std::size_t fi = 0; fi < mesh.faceCount(); ++fi) {
    const auto &face = mesh.faces[fi];

    for (int i = 0; i < 3; ++i) {
      const Edge edge = makeUndirectedEdge(face[i], face[(i + 1) % 3]);
      const auto existing = edge_to_face.find(edge);

      if (existing == edge_to_face.end()) {
        edge_to_face.emplace(edge, fi);
      } else {
        const std::size_t neighbor_face = existing->second;
        adjacency[fi].push_back(neighbor_face);
        adjacency[neighbor_face].push_back(fi);
      }
    }
  }

  return adjacency;
}

FaceLabels segmentByNormalRegionGrowing(const TriangleMesh &mesh,
                                        double max_normal_angle_degrees) {
  const auto face_features = computeFaceFeatures(mesh);
  const auto adjacency = computeFaceAdjacency(mesh);

  FaceLabels labels = std::vector<int>(mesh.faceCount(), -1);
  int current_label = 0;

  for (std::size_t fi = 0; fi < mesh.faceCount(); ++fi) {
    if (labels[fi] != -1) {
      continue;
    }
    std::queue<std::size_t> queue;
    queue.push(fi);
    labels[fi] = current_label;
    while (!queue.empty()) {
      const auto qi = queue.front();
      queue.pop();
      for (const auto neighbor : adjacency[qi]) {
        double angle = std::clamp(
            face_features[qi].normal.dot(face_features[neighbor].normal), -1.0,
            1.0);
        angle = std::acos(angle) * 180.0 / M_PI;
        if (labels[neighbor] == -1 && angle < max_normal_angle_degrees) {
          labels[neighbor] = current_label;
          queue.push(neighbor);
        }
      }
    }
    ++current_label;
  }
  return labels;
}

} // namespace dental::mesh
