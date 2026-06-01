#include "dental/io/MeshIO.hpp"
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
  return 0;
}