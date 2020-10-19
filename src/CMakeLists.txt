aux_source_directory(. DIR_SRCS)

add_executable(xvaparse ${DIR_SRCS})

target_include_directories(xvaparse PUBLIC ../dep/newtar ../dep/pugixml)

target_link_libraries(xvaparse PUBLIC 
	#${PROJECT_SOURCE_DIR}/libs/libmytar.a
	pugixml
	mytar
)
