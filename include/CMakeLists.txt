### Get the headers paths
SET(include_list "${CMAKE_CURRENT_SOURCE_DIR}/dynamic-graph-API.h")
FILE(GLOB_RECURSE headers_list 
	${PROJECT_SOURCE_DIR}/src/*.h
)
FILE(GLOB_RECURSE templates_list 
	${PROJECT_SOURCE_DIR}/src/*.t.cpp
)
LIST(APPEND include_list ${headers_list})
LIST(APPEND include_list ${templates_list})

### Copy headers to include directories
SET(HEADER_IN_INCLUDE_DIR "")
FOREACH(header ${include_list})
	GET_FILENAME_COMPONENT(headerName ${header} NAME)
	
	ADD_CUSTOM_COMMAND(
	   OUTPUT  ${${PROJECT_NAME}_BINARY_DIR}/include/${PROJECT_NAME}/${headerName}
	   COMMAND ${CMAKE_COMMAND} -E copy ${header}
	   		${${PROJECT_NAME}_BINARY_DIR}/include/${PROJECT_NAME}/${headerName}
	)
	
	INSTALL(FILES
	   ${${PROJECT_NAME}_BINARY_DIR}/include/${PROJECT_NAME}/${headerName}
	   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}
	   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
  	
  	SET(HEADER_IN_INCLUDE_DIR ${HEADER_IN_INCLUDE_DIR} 
       ${${PROJECT_NAME}_BINARY_DIR}/include/${PROJECT_NAME}/${headerName})
ENDFOREACH(header)

### Create a top level rule to copy all the headers from src to include/dynamic-graph
ADD_CUSTOM_TARGET(headers ALL
		   DEPENDS ${HEADER_IN_INCLUDE_DIR})

### Dependency of the main library on the headers
ADD_DEPENDENCIES(${PROJECT_NAME}
	headers)
