#include "dental/io/MeshIO.hpp"
#include "dental/mesh/MeshSegmentation.hpp"
#include "dental/mesh/MeshStats.hpp"
#include "dental/mesh/TriangleMesh.hpp"

#include <cmath>
#include <filesystem>
#include <stdexcept>

namespace {

void require(bool condition, const char *message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

void requireNear(double actual, double expected, const char *message) {
  constexpr double epsilon = 1e-9;
  if (std::abs(actual - expected) > epsilon) {
    throw std::runtime_error(message);
  }
}

void requireVectorNear(const Eigen::Vector3d &actual,
                       const Eigen::Vector3d &expected,
                       const char *message) {
  constexpr double epsilon = 1e-9;
  if ((actual - expected).norm() > epsilon) {
    throw std::runtime_error(message);
  }
}

} // namespace

int main() {
  Eigen::Vector3d point{1.0, 2.0, 3.0};
  require(point.z() == 3.0, "Eigen vector sanity check failed");

  dental::mesh::TriangleMesh triangle;
  triangle.vertices = {
      Eigen::Vector3d{0.0, 0.0, 0.0},
      Eigen::Vector3d{1.0, 0.0, 0.0},
      Eigen::Vector3d{0.0, 1.0, 0.0},
  };
  triangle.faces = {{{0, 1, 2}}};

  auto bbox = dental::mesh::computeBoundingBox(triangle);
  require(bbox.min.isApprox(Eigen::Vector3d{0.0, 0.0, 0.0}),
          "Triangle bounding box min is wrong");
  require(bbox.max.isApprox(Eigen::Vector3d{1.0, 1.0, 0.0}),
          "Triangle bounding box max is wrong");
  requireVectorNear(dental::mesh::computeCentroid(triangle),
                    Eigen::Vector3d{1.0 / 3.0, 1.0 / 3.0, 0.0},
                    "Triangle centroid is wrong");
  requireNear(dental::mesh::computeSurfaceArea(triangle), 0.5,
              "Triangle surface area is wrong");
  requireNear(dental::mesh::computeAverageEdgeLength(triangle),
              (2.0 + std::sqrt(2.0)) / 3.0,
              "Triangle average edge length is wrong");

  const Eigen::Vector3d centroid = dental::mesh::computeCentroid(triangle);
  for (auto &vertex : triangle.vertices) {
    vertex -= centroid;
  }
  requireVectorNear(dental::mesh::computeCentroid(triangle),
                    Eigen::Vector3d::Zero(),
                    "Centered triangle centroid should be zero");

  dental::mesh::TriangleMesh square;
  square.vertices = {
      Eigen::Vector3d{0.0, 0.0, 0.0},
      Eigen::Vector3d{1.0, 0.0, 0.0},
      Eigen::Vector3d{1.0, 1.0, 0.0},
      Eigen::Vector3d{0.0, 1.0, 0.0},
  };
  square.faces = {{{0, 1, 2}}, {{0, 2, 3}}};

  const dental::mesh::FaceAdjacency adjacency =
      dental::mesh::computeFaceAdjacency(square);
  require(adjacency.size() == 2, "Square adjacency should have two faces");
  require(adjacency[0].size() == 1, "First square face should have one neighbor");
  require(adjacency[1].size() == 1, "Second square face should have one neighbor");
  require(adjacency[0][0] == 1, "First square face neighbor should be face 1");
  require(adjacency[1][0] == 0, "Second square face neighbor should be face 0");

  const auto square_features = dental::mesh::computeFaceFeatures(square);
  require(square_features.size() == square.faceCount(),
          "Feature count should match face count");

  const dental::mesh::FaceLabels square_labels =
      dental::mesh::segmentByNormalRegionGrowing(square, 10.0);
  require(square_labels.size() == square.faceCount(),
          "Label count should match face count");
  require(square_labels[0] == square_labels[1],
          "Coplanar square triangles should be one region");

  dental::mesh::TriangleMesh folded;
  folded.vertices = {
      Eigen::Vector3d{0.0, 0.0, 0.0},
      Eigen::Vector3d{1.0, 0.0, 0.0},
      Eigen::Vector3d{0.0, 1.0, 0.0},
      Eigen::Vector3d{0.0, 0.0, 1.0},
  };
  folded.faces = {{{0, 1, 2}}, {{0, 3, 1}}};

  const dental::mesh::FaceLabels folded_labels =
      dental::mesh::segmentByNormalRegionGrowing(folded, 45.0);
  require(folded_labels.size() == folded.faceCount(),
          "Folded label count should match face count");
  require(folded_labels[0] != folded_labels[1],
          "Perpendicular triangles should split with 45 degree threshold");

  dental::mesh::TriangleMesh mesh =
      dental::io::loadSTL("data/models/bodyparts3d_tooth.stl");
  require(mesh.vertexCount() > 0, "Loaded STL should have vertices");
  require(mesh.faceCount() > 0, "Loaded STL should have faces");
  require(dental::mesh::computeSurfaceArea(mesh) > 0.0,
          "Loaded STL should have positive surface area");

  const std::filesystem::path saved_path =
      std::filesystem::temp_directory_path() / "dental_saved_mesh_test.stl";
  dental::io::saveMesh(mesh, saved_path.string());

  dental::mesh::TriangleMesh reloaded = dental::io::loadSTL(saved_path.string());
  require(reloaded.vertexCount() > 0, "Reloaded STL should have vertices");
  require(reloaded.faceCount() > 0, "Reloaded STL should have faces");
  require(dental::mesh::computeSurfaceArea(reloaded) > 0.0,
          "Reloaded STL should have positive surface area");

  std::filesystem::remove(saved_path);

  return 0;
}
