# Copyright 2010, Olivier Stasse, JRL, CNRS/AIST
#
# This file is part of dynamic-graph.
# dynamic-graph is free software: 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
