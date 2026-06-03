#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <Eigen/Core>

namespace dental::mesh {

struct TriangleMesh {
  std::vector<Eigen::Vector3d> vertices;
  std::vector<std::array<std::uint32_t, 3>> faces;

  std::size_t vertexCount() const { return vertices.size(); }
  std::size_t faceCount() const { return faces.size(); }
  bool empty() const { return vertices.empty(); }
};

} // namespace dental::mesh
