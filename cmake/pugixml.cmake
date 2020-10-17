set(PUGIXML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/pugixml)
include_directories("${PUGIXML_DIR}")
add_subdirectory(${PUGIXML_DIR} dep/pugixml)

