AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(xvaparse ${DIR_SRCS})

TARGET_INCLUDE_DIRECTORIES(xvaparse PUBLIC ../dep/newtar ../dep/pugixml)

TARGET_LINK_LIBRARIES(xvaparse PUBLIC 
	#${PROJECT_SOURCE_DIR}/libs/libmytar.a
	pugixml
	mytar
)
