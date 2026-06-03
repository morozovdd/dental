#pragma once

#include "dental/mesh/TriangleMesh.hpp"

namespace dental::mesh {

TriangleMesh weldVertices(const TriangleMesh &mesh, double epsilon);

}