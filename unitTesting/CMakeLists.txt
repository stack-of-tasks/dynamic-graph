# 
#  Copyright 
# 

### tests
SET(tests
	test_shell
	test_pool
	test_depend
	test_factory)

FOREACH(test_name ${tests})
	SET(EXECUTABLE_NAME ${test_name})

	ADD_DEFINITIONS(-DDEBUG=2)

	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${test_name}.cpp)
	
	INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_SOURCE_DIR}/../include 
	)

	LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/lib)

	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  ${PROJECT_NAME}
	  dl)
	  
	INSTALL(TARGETS ${test_name}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME})
ENDFOREACH(test_name)