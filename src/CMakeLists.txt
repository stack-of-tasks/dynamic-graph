# Copyright 2010, Olivier Stasse, JRL, CNRS/AIST
#
# This file is part of dynamic-graph.
# dynamic-graph is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# dynamic-graph is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# dynamic-graph. If not, see <http://www.gnu.org/licenses/>.

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

  dgraph/entity.cpp
  dgraph/factory.cpp
  dgraph/pool.cpp

  exception/exception-abstract.cpp
  exception/exception-factory.cpp
  exception/exception-signal.cpp
  exception/exception-traces.cpp

  signal/signal-array.cpp
  signal/signal-caster.cpp
  signal/signal-cast-helper.cpp

  command/value.cpp
  command/command.cpp
  )

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})

PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} jrl-mal)

IF (UNIX)
  TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${CMAKE_DL_LIBS} pthread)
ENDIF (UNIX)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${JRL_MAL_LDFLAGS_OTHER})

INSTALL(TARGETS ${LIBRARY_NAME}
  DESTINATION lib)

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
