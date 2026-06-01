#include "dental/io/MeshIO.hpp"
#include "dental/mesh/TriangleMesh.hpp"
#include <iostream>

#include <stdexcept>

namespace {

void require(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

} // namespace

int main() {
  Eigen::Vector3d point{1.0, 2.0, 3.0};
  require(point.z() == 3.0, "Eigen vector sanity check failed");

  dental::mesh::TriangleMesh mesh =
      dental::io::loadSTL("data/models/bodyparts3d_tooth.stl");
  require(mesh.vertexCount() > 0, "Loaded STL should have vertices");
  require(mesh.faceCount() > 0, "Loaded STL should have faces");

  return 0;
}
