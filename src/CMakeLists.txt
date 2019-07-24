# Copyright 2010, Olivier Stasse, JRL, CNRS/AIST
#

####################################
### Main Library
####################################
SET(LIBRARY_NAME ${PROJECT_NAME})

# Verbosity level
IF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))
  ADD_DEFINITIONS(-DVP_DEBUG_MODE=${CMAKE_VERBOSITY_LEVEL} -DVP_DEBUG)
ENDIF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))

# Declare boost include directories
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  debug/debug.cpp
  debug/real-time-logger.cpp
  debug/logger.cpp

  dgraph/entity.cpp
  dgraph/factory.cpp
  dgraph/pool.cpp

  exception/exception-abstract.cpp
  exception/exception-factory.cpp
  exception/exception-signal.cpp
  exception/exception-traces.cpp

  mt/process-list.cpp

  signal/signal-array.cpp
  signal/signal-caster.cpp
  signal/signal-cast-helper.cpp

  command/value.cpp
  command/command.cpp
  )

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})


IF (UNIX)
  TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${CMAKE_DL_LIBS} pthread)
ENDIF (UNIX)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${Boost_LIBRARIES})

#IF (UNIX AND NOT APPLE)
#  TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${JRL_MAL_LDFLAGS_OTHER})
#ENDIF (UNIX AND NOT APPLE)

INSTALL(TARGETS ${LIBRARY_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR})

####################################
### Plugins
####################################
SET(plugins_list
	traces/tracer
	traces/tracer-real-time
)

SET(tracer-real-time_dependency tracer)

FOREACH(plugin_file ${plugins_list})
  GET_FILENAME_COMPONENT(plugin ${plugin_file} NAME)
  ADD_LIBRARY(${plugin} SHARED "${plugin_file}.cpp")

  TARGET_LINK_LIBRARIES(${plugin}
    ${PROJECT_NAME} ${${plugin}_dependency}
    ${Boost_LIBRARIES})

  SET_TARGET_PROPERTIES(${plugin}
    PROPERTIES
    PREFIX ""
    )

  INSTALL(TARGETS ${plugin} DESTINATION ${PLUGINDIR})
ENDFOREACH(plugin_file)
