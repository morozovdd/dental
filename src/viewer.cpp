#include "dental/io/MeshIO.hpp"
#include "dental/mesh/MeshSegmentation.hpp"

#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>

int main(int argc, char **argv) {
  if (argc != 2)
    return 1;

  auto mesh = dental::io::loadSTL(argv[1]);

  auto labels = dental::mesh::segmentByNormalRegionGrowing(mesh, 40.0);

  polyscope::init();
  polyscope::registerSurfaceMesh("teeth", mesh.vertices, mesh.faces);

  auto *ps_mesh =
      polyscope::registerSurfaceMesh("teeth", mesh.vertices, mesh.faces);

  ps_mesh->addFaceScalarQuantity("segments", labels);

  // print number of lables
  std::cout << "Number of segments: "
            << std ::unique(labels.begin(), labels.end()) - labels.begin()
            << std::endl;

  polyscope::show();

  return 0;
}