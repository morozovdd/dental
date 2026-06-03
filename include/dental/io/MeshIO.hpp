#pragma once

#include "dental/mesh/TriangleMesh.hpp"
#include <string>

namespace dental::io {

dental::mesh::TriangleMesh loadMesh(const std::string &path);
dental::mesh::TriangleMesh loadSTL(const std::string &path);

void saveMesh(const dental::mesh::TriangleMesh &mesh, const std::string &path);
} // namespace dental::io