cmake_minimum_required(VERSION 3.0)
project(lucyrt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fvisibility=hidden -std=c++11")

include_directories(
  vendors/glad-3.3/include/
  vendors/glfw-3.3/include/
  src/)
add_subdirectory(vendors)

file(GLOB SOURCES src/*.cc)
source_group("Sources" FILES ${SOURCES})
add_executable(lucyrt ${SOURCES})
target_link_libraries(${PROJECT_NAME} vendors)
