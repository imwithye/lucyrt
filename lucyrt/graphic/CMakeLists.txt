file(GLOB_RECURSE SOURCES *.cc)
source_group("Sources" FILES ${SOURCES})
add_library(graphic STATIC ${SOURCES})
