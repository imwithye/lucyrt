cmake_minimum_required(VERSION 3.1)
project(lucyrt)

set(ASSIMP_DIR assimp-5.0.0)
set(GLAD_DIR glad-3.3)
set(GLFW_DIR glfw-3.3)
set(GLM_DIR glm-0.9.9.6)
set(IMGUI_DIR imgui-1.73)
set(JSON_DIR json-3.7.0)
set(SPDLOG_DIR spdlog-1.4.2)
set(SQLITE_DIR sqlite-amalgamation-3300100)
set(STB_IMAGE_DIR stb_image-2.23)

add_subdirectory(vendors)
include_directories(
  vendors/${GLAD_DIR}/include/
  vendors/${GLFW_DIR}/include/
  vendors/${GLM_DIR}/
  vendors/${IMGUI_DIR}/
  vendors/${IMGUI_DIR}/examples/
  vendors/${JSON_DIR}/
  vendors/${SPDLOG_DIR}/include/spdlog/
  vendors/${SQLITE_DIR}/
  vendors/${STB_IMAGE_DIR}/
  src/Sources/)

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fvisibility=hidden")
endif()

file(GLOB_RECURSE PROJECT_SOURCES src/Sources/*.cc)
file(GLOB_RECURSE PROJECT_RESOURCES src/Shaders/*.vert
  src/Shaders/*.frag src/Shaders/*.geom src/Shaders/*.comp
  src/Textures/*.png src/Textures/*.jpg src/Textures/*.tga)
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Resources" FILES ${PROJECT_RESOURCES})

add_executable(rsgen rsgen.cc)
set(Resources ${CMAKE_SOURCE_DIR}/src/Sources/resource/rs.h)
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
