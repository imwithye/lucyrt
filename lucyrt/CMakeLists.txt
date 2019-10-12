set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fvisibility=hidden -std=c++11")

add_subdirectory(graphic)

file(GLOB SOURCES *.cc)
source_group("Sources" FILES ${SOURCES})
add_executable(lucyrt ${SOURCES})
target_link_libraries(${PROJECT_NAME} graphic)
