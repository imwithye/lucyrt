set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -fvisibility=hidden -std=c++11")

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp-5.0.0)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw-3.3)

add_subdirectory(spdlog-1.4.2)

include_directories(imgui-1.73/examples)
file(GLOB SOURCES glad-3.3/src/glad.c
  imgui-1.73/*.cpp imgui-1.73/examples/imgui_impl_glfw.cpp imgui-1.73/examples/imgui_impl_opengl3.cpp)
source_group("Sources" FILES ${SOURCES})
add_definitions(-DGLFW_INCLUDE_NONE)

add_library(vendors STATIC ${SOURCES})
target_include_directories(assimp PUBLIC
                  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/assimp/include>
                  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/assimp/include>)
target_link_libraries(vendors assimp glfw ${GLFW_LIBRARIES} spdlog)
