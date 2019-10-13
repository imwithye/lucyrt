cmake_minimum_required(VERSION 3.0)
project(lucyrt)

include_directories(
  vendors/glad-3.3/include/
  vendors/glfw-3.3/include/
  vendors/glm-0.9.9.6/
  vendors/imgui-1.73/
  vendors/imgui-1.73/examples/
  src/Sources/)
add_subdirectory(vendors)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fvisibility=hidden -std=c++11")

file(GLOB_RECURSE PROJECT_SOURCES src/Sources/*.cc)
file(GLOB_RECURSE PROJECT_RESOURCES src/Shaders/*.vert
  src/Shaders/*.frag
  src/Shaders/*.geom
  src/Shaders/*.comp)
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Resources" FILES ${PROJECT_RESOURCES})

add_executable(rsgen rsgen.cc)
set(Resources ${CMAKE_SOURCE_DIR}/src/Sources/rs.h)
add_custom_command(OUTPUT ${Resources}
  COMMAND rsgen ${Resources} ${CMAKE_SOURCE_DIR}/src ${PROJECT_RESOURCES}
  DEPENDS ${PROJECT_RESOURCES}
  DEPENDS rsgen.cc
  COMMENT "Generating resources"
  VERBATIM)
add_custom_target(rsgen-gen
  DEPENDS ${Resources}
  DEPENDS rsgen.cc)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_dependencies(${PROJECT_NAME} rsgen-gen)
target_link_libraries(${PROJECT_NAME} vendors)
