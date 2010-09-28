# 
#  Copyright 
# 

### tools
SET(tools
	sot-shell
)

### Configure shell launch script
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/sot-shell-plugin.sh.cmake
	       ${${PROJECT_NAME}_BINARY_DIR}/tools/sot-shell-plugin.sh)
INSTALL(
	FILES
		${${PROJECT_NAME}_BINARY_DIR}/tools/sot-shell-plugin.sh
	DESTINATION
		 bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                      GROUP_EXECUTE GROUP_READ )

FOREACH(tool_name ${tools})
	SET(EXECUTABLE_NAME ${tool_name})

	ADD_DEFINITIONS(-DDEBUG=2)

	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${tool_name}.cpp)
	
	INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_SOURCE_DIR}/../include 
	)

	LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/lib)

	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  ${PROJECT_NAME})
	  
	IF(UNIX)
		TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} dl)
	ENDIF(UNIX)
	  
	INSTALL(TARGETS ${tool_name} DESTINATION bin)
ENDFOREACH(tool_name)
