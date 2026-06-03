#include "dental/io/MeshIO.hpp"
#include "dental/mesh/TriangleMesh.hpp"
#include <algorithm>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
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
  std::transform(
      lowercase_path.begin(), lowercase_path.end(), lowercase_path.begin(),
      [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

  if (lowercase_path.size() < 4 ||
      lowercase_path.substr(lowercase_path.size() - 4) != ".stl") {
    throw std::runtime_error("Invalid file extension. Expected .stl but got " +
                             path);
  }
  return loadMesh(path);
}

void saveMesh(const dental::mesh::TriangleMesh &mesh, const std::string &path) {
  if (mesh.empty()) {
    throw std::runtime_error("Cannot save empty mesh");
  }
  if (mesh.faceCount() == 0) {
    throw std::runtime_error("Cannot save mesh with no faces");
  }

  aiScene scene;
  scene.mRootNode = new aiNode();
  scene.mNumMeshes = 1;
  scene.mMeshes = new aiMesh *[1];
  scene.mNumMaterials = 1;
  scene.mMaterials = new aiMaterial *[1];
  scene.mMaterials[0] = new aiMaterial();

  aiMesh *ai_mesh = new aiMesh();
  ai_mesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;
  ai_mesh->mMaterialIndex = 0;
  ai_mesh->mNumVertices = static_cast<unsigned int>(mesh.vertexCount());
  ai_mesh->mVertices = new aiVector3D[ai_mesh->mNumVertices];

  for (std::size_t i = 0; i < mesh.vertexCount(); ++i) {
    const auto &v = mesh.vertices[i];
    ai_mesh->mVertices[i] = aiVector3D(static_cast<float>(v.x()),
                                       static_cast<float>(v.y()),
                                       static_cast<float>(v.z()));
  }

  ai_mesh->mNumFaces = static_cast<unsigned int>(mesh.faceCount());
  ai_mesh->mFaces = new aiFace[ai_mesh->mNumFaces];

  for (std::size_t i = 0; i < mesh.faceCount(); ++i) {
    const auto &f = mesh.faces[i];
    for (std::uint32_t index : f) {
      if (index >= mesh.vertexCount()) {
        throw std::runtime_error("Mesh face references a missing vertex");
      }
    }

    aiFace &face = ai_mesh->mFaces[i];
    face.mNumIndices = 3;
    face.mIndices = new unsigned int[3];
    face.mIndices[0] = static_cast<unsigned int>(f[0]);
    face.mIndices[1] = static_cast<unsigned int>(f[1]);
    face.mIndices[2] = static_cast<unsigned int>(f[2]);
  }

  scene.mMeshes[0] = ai_mesh;

  scene.mRootNode->mNumMeshes = 1;
  scene.mRootNode->mMeshes = new unsigned int[1];
  scene.mRootNode->mMeshes[0] = 0;

  Assimp::Exporter exporter;
  aiReturn result = exporter.Export(&scene, "stl", path);
  if (result != aiReturn_SUCCESS) {
    throw std::runtime_error(exporter.GetErrorString());
  }
}
} // namespace dental::io
