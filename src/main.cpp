#include "dental/io/MeshIO.hpp"
#include "dental/mesh/MeshStats.hpp"
#include <iostream>

int main(const int argc, const char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <path_to_mesh_file>" << std ::endl;
    return 1;
  }
  // 1. Load mesh from STL file
  std::string mesh_path = argv[1];
  auto mesh = dental::io::loadSTL(mesh_path);

  // 2. Print some information about the loaded mesh
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
  return 0;
}