if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -fvisibility=hidden -std=c++11")
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
endif()

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(${ASSIMP_DIR})

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(${GLFW_DIR})

add_subdirectory(${SPDLOG_DIR})

include_directories(${IMGUI_DIR}/ ${IMGUI_DIR}/examples/ ${GLAD_DIR}/include/)
file(GLOB SOURCES glad-3.3/src/glad.c
  ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/examples/imgui_impl_glfw.cpp ${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
  ${SQLITE_DIR}/*.c)
source_group("Sources" FILES ${SOURCES})
add_definitions(-DGLFW_INCLUDE_NONE)

add_library(vendors STATIC ${SOURCES})
target_include_directories(assimp PUBLIC
                  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/assimp/include>
                  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/assimp/include>)
target_link_libraries(vendors assimp glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} spdlog)
