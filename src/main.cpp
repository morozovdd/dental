#include "dental/io/MeshIO.hpp"
#include "dental/mesh/MeshStats.hpp"
#include <iostream>
#include <stdexcept>

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path_to_mesh_file>" << std ::endl;
    return 1;
  }
  // 1. Load mesh from STL file
  std::string mesh_path = argv[1];
  auto mesh = dental::io::loadSTL(mesh_path);

  // 2. Print stats of loaded mesh
  std::cout << "Loaded mesh with " << mesh.vertexCount() << " vertices and "
            << mesh.faceCount() << " faces." << std::endl;

  auto bbox = dental::mesh::computeBoundingBox(mesh);
  std::cout << "Bounding box: min=" << bbox.min << ", max=" << bbox.max
            << std::endl;

  auto centroid = dental::mesh::computeCentroid(mesh);
  std::cout << "Centroid: " << centroid << std::endl;

  auto surface_area = dental::mesh::computeSurfaceArea(mesh);
  std::cout << "Surface area: " << surface_area << std::endl;

  auto avg_edge_length = dental::mesh::computeAverageEdgeLength(mesh);
  std::cout << "Average edge length: " << avg_edge_length << std::endl;

  // 3. Bring mesh to the origin
  // 3.1 Center and compute covariance matrix
  Eigen::Matrix3d covariance = Eigen::Matrix3d::Zero();
  for (const auto &v : mesh.vertices) {
    Eigen::Vector3d centered = v - centroid;
    covariance += centered * centered.transpose();
  }

  covariance /= static_cast<double>(mesh.vertexCount());

  // 3.2 Eigendecompose
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(covariance);
  if (solver.info() != Eigen::Success) {
    throw std::runtime_error("Failed to eigendecompose covariance matrix");
  }

  Eigen::Vector3d eigenvalues =
      solver.eigenvalues(); // returned in increasing order
  Eigen::Matrix3d eigenvectors = solver.eigenvectors();

  // world = R * pca;
  // pca   = R.transpose() * world;
  for (auto &v : mesh.vertices) {
    v = eigenvectors.transpose() * (v - centroid);
  }

  std::cout << eigenvalues << std::endl;
  auto new_centroid = dental::mesh::computeCentroid(mesh);
  std::cout << "New centroid" << new_centroid.transpose() << std::endl;

  // // 4. Save the mesh
  // dental::io::saveMesh(mesh, "transformed.stl");

  return 0;
}
