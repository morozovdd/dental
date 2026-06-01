#include "dental/io/MeshIO.hpp"
#include "dental/mesh/TriangleMesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace dental::io {

dental::mesh::TriangleMesh loadMesh(const std::string &path) {
  mesh::TriangleMesh mesh;
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                aiProcess_ValidateDataStructure);

  if (nullptr == scene) {
    throw std::runtime_error(importer.GetErrorString());
  }

  if (!scene->HasMeshes()) {
    throw std::runtime_error("No meshes found in file.");
  }
  const aiMesh *ai_mesh = scene->mMeshes[0];

  for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
    const aiVector3D &v = ai_mesh->mVertices[i];
    mesh.vertices.push_back({v.x, v.y, v.z});
  }

  for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
    const aiFace &face = ai_mesh->mFaces[i];
    if (3 != face.mNumIndices) {
      throw std::runtime_error("Non-triangular face found.");
    }
    mesh.faces.push_back(
        {face.mIndices[0], face.mIndices[1], face.mIndices[2]});
  }
  return mesh;
}

dental::mesh::TriangleMesh loadSTL(const std::string &path) {
  if (path.empty()) {
    throw std::runtime_error("Empty file path provided");
  }

  std::string lowercase_path = path;
  std::transform(lowercase_path.begin(), lowercase_path.end(),
                 lowercase_path.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

  if (lowercase_path.size() < 4 ||
      lowercase_path.substr(lowercase_path.size() - 4) != ".stl") {
    throw std::runtime_error("Invalid file extension. Expected .stl but got " +
                             path);
  }
  return loadMesh(path);
}
} // namespace dental::io
