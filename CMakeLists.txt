cmake_minimum_required(VERSION 3.10)

project(myxva VERSION 1.0 DESCRIPTION "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(cmake/mytar.cmake)
include(cmake/pugixml.cmake)

SET(SPATH "src")

configure_file(
        ${PROJECT_SOURCE_DIR}/Configure.h.in
        ${PROJECT_SOURCE_DIR}/${SPATH}/Configure.h
        @ONLY
)

add_subdirectory(${SPATH})

cmake_dependent_option(BUILD_TESTS
  "Build tests" ON
  "BUILD_TESTING;CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF)

if(BUILD_TESTS) 
	set(test-pattern "test/*.cc")
	file(GLOB test-sources ${test-pattern})
	add_executable(xml-test ${test-sources})
	target_include_directories(xml-test PUBLIC ./dep/pugixml)
	target_link_libraries(xml-test pugixml)
endif(BUILD_TESTS)
