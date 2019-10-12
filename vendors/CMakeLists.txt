set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++11")

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw-3.3)

include_directories(imgui-1.73/examples)
file(GLOB SOURCES glad-3.3/src/glad.c
  imgui-1.73/*.cpp imgui-1.73/examples/imgui_impl_glfw.cpp imgui-1.73/examples/imgui_impl_opengl3.cpp)
source_group("Sources" FILES ${SOURCES})
add_definitions(-DGLFW_INCLUDE_NONE)

add_library(vendors STATIC ${SOURCES})
target_link_libraries(vendors glfw ${GLFW_LIBRARIES})
