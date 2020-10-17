set(NEWTAR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/newtar)
include_directories("${NEWTAR_DIR}")
add_subdirectory(${NEWTAR_DIR} dep/newtar)

