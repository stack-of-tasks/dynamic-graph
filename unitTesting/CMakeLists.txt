# 
#  Copyright 
# 

SET(EXECUTABLE_NAME test_shell)

ADD_DEFINITIONS(-DDEBUG=2)

ADD_EXECUTABLE(${EXECUTABLE_NAME}
  test_shell.cpp)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

link_directories(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
  dynamicGraph
  dl)


#NOTE on test_factory:
# test can only be compiled IF sotGainAdaptive has been compiled first
SET(EXECUTABLE_NAME test_factory)

ADD_DEFINITIONS(-DDEBUG=2)

ADD_EXECUTABLE(${EXECUTABLE_NAME}
  test_factory.cpp)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

link_directories(${LIBRARY_OUTPUT_PATH})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
  dynamicGraph
  dl)