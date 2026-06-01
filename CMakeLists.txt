cmake_minimum_required(VERSION 3.10)
project(dental VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Eigen3 REQUIRED NO_MODULE)

add_library(dental_core INTERFACE)
target_include_directories(dental_core INTERFACE include)
target_link_libraries(dental_core INTERFACE Eigen3::Eigen)

find_package(assimp REQUIRED)
target_link_libraries(dental_core INTERFACE assimp::assimp)

add_library(dental_io
  src/io/MeshIO.cpp
)

target_include_directories(dental_io PUBLIC include)
target_link_libraries(dental_io PUBLIC dental_core)
target_compile_options(dental_io PRIVATE -Wall -Wextra -pedantic)

add_library(dental_geometry
  src/geometry/GeometryAlgorithms.cpp
  src/geometry/Line3D.cpp
  src/geometry/Plane.cpp
  src/geometry/Vec3.cpp
)

target_include_directories(dental_geometry PUBLIC include)
target_compile_options(dental_geometry PRIVATE -Wall -Wextra -pedantic)

add_executable(app
  src/main.cpp
)

target_link_libraries(app PRIVATE dental_io dental_geometry)
target_compile_options(app PRIVATE -Wall -Wextra -pedantic)

include(CTest)

if(BUILD_TESTING)
  add_executable(geometry_tests
    tests/GeometryTests.cpp
  )

  target_link_libraries(geometry_tests PRIVATE dental_geometry)
  target_compile_options(geometry_tests PRIVATE -Wall -Wextra -pedantic)

  add_test(NAME geometry_tests COMMAND geometry_tests)

  add_executable(mesh_tests
    tests/MeshTests.cpp
  )

  target_link_libraries(mesh_tests PRIVATE dental_io)
  target_compile_options(mesh_tests PRIVATE -Wall -Wextra -pedantic)

  add_test(NAME mesh_tests COMMAND mesh_tests)
  set_tests_properties(mesh_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

endif()
