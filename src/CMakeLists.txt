####################################
### Main Library
####################################
SET(LIBRARY_NAME ${PROJECT_NAME})

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Verbosity level

IF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))
  ADD_DEFINITIONS(-DVP_DEBUG_MODE=${CMAKE_VERBOSITY_LEVEL} -DVP_DEBUG)
ENDIF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))

ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  debug/debug.cpp

  dgraph/entity.cpp
  dgraph/factory.cpp
  dgraph/interpreter.cpp
  dgraph/interpreter-helper.cpp
  dgraph/plugin-loader.cpp
  dgraph/pool.cpp

  exception/exception-abstract.cpp
  exception/exception-factory.cpp
  exception/exception-signal.cpp
  exception/exception-traces.cpp

  shell/functions.cpp

  signal/signal-array.cpp
  signal/signal-caster.cpp
  )

SET_TARGET_PROPERTIES(${LIBRARY_NAME}
  PROPERTIES
  SOVERSION ${PROJECT_VERSION}
  INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

SET_TARGET_PROPERTIES(${LIBRARY_NAME}
  PROPERTIES
  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS}"
)

IF (UNIX)
  TARGET_LINK_LIBRARIES(${LIBRARY_NAME} dl pthread) 
ENDIF (UNIX)

INSTALL(TARGETS ${LIBRARY_NAME}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

####################################
### Plugins
####################################
SET(plugins_list
	plugins/shell-procedure
	plugins/shell-functions
	debug/contiifstream
	traces/tracer
	traces/tracer-real-time
)

SET(tracer-real-time_dependancy tracer)

FOREACH(plugin_file ${plugins_list})
GET_FILENAME_COMPONENT(plugin ${plugin_file} NAME)
ADD_LIBRARY(${plugin}
 SHARED
 "${plugin_file}.cpp")

TARGET_LINK_LIBRARIES(${plugin}
	${PROJECT_NAME} ${${plugin}_dependancy})

SET_TARGET_PROPERTIES(${plugin}
   PROPERTIES
   COMPILE_FLAGS "${${PROJECT_NAME}_CXXFLAGS}"
   PREFIX ""
   LIBRARY_OUTPUT_DIRECTORY "${${PROJECT_NAME}_BINARY_DIR}/lib/plugin")

INSTALL(TARGETS ${plugin}
		DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plugin)
ENDFOREACH(plugin_file)