# 
#  Copyright 
# 

### tests
SET(tests_exe
	test_pool
	test_depend
	test_signalcast)

SET(tests_libs
	test_signalcast_libA
	test_signalcast_libB
)

SET(test_signalcast_additional_libs ${test_libs})

ADD_DEFINITIONS(-DDEBUG=2)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../include 
)

LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/lib)
	
# Additional tests
FOREACH(test_name ${tests_libs})
	SET(LIBRARY_NAME ${test_name})

	ADD_LIBRARY(${LIBRARY_NAME}
	  SHARED
	  ${test_name}.cpp)
	
	TARGET_LINK_LIBRARIES(${LIBRARY_NAME}
	  ${PROJECT_NAME})
ENDFOREACH(test_name)

FOREACH(test_name ${tests_exe})
	SET(EXECUTABLE_NAME ${test_name})

	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${test_name}.cpp)
		
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  ${PROJECT_NAME})
	  
	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} 
		${${test_name}_additional_libs})
	  
	ADD_TEST(NAME ${test_name} COMMAND ${test_name})
ENDFOREACH(test_name)