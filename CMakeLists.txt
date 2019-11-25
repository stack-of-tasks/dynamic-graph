# Copyright 2010, Olivier Stasse, JRL, CNRS/AIST
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECT_ORG stack-of-tasks)
SET(PROJECT_NAME dynamic-graph)
SET(PROJECT_DESCRIPTION "Dynamic graph library")
SET(PROJECT_URL "http://github.com/${PROJECT_ORG}/${PROJECT_NAME}")
SET(PROJECT_SUFFIX "-v3")

# Export CMake Target
SET(PROJECT_USE_CMAKE_EXPORT TRUE)
# Make sure that every header is generated in dynamic-graph
SET(CUSTOM_HEADER_DIR ${PROJECT_NAME})

INCLUDE(cmake/base.cmake)

# Specify the project.
cmake_policy(SET CMP0048 NEW)
PROJECT(${PROJECT_NAME}
  LANGUAGES
  CXX
  VERSION
  ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
  )


# jrl-cmakemodules handling of usual modules
INCLUDE(cmake/header.cmake)
INCLUDE(cmake/pthread.cmake)
add_project_dependency(Eigen3 REQUIRED NO_MODULE)
add_project_dependency(Boost REQUIRED
  OPTIONAL_COMPONENTS
  serialization system unit_test_framework thread)
# Allows math in mathjax.
SET(DOXYGEN_USE_MATHJAX YES)

# Defines paths.
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Add configuration headers for plug-ins.
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR} config-tracer.hh DG_TRACER tracer_EXPORTS)
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR}
  config-tracer-real-time.hh DG_TRACERREALTIME tracer_real_time_EXPORTS)

set(PACKAGE_EXTRA_MACROS
  "set (DYNAMIC_GRAPH_PLUGINDIR \${CMAKE_CURRENT_LIST_DIR}/../../plugin)")

# Specific to PKG module
# FIXME: to be changed into lib/dynamic-graph
# to avoid name collision whenX installing dynamic-graph in /usr.
SET(PLUGINDIR "${CMAKE_INSTALL_FULL_LIBDIR}/plugin")

# Add extra keys to pkg-config file to export the location of the
# plug-in directory in a robust manner.
SET(PKG_CONFIG_EXTRA "plugindir=${PLUGINDIR}")

# Add dependency toward dynamic graph library in pkg-config file.
PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

# Search for Boost.
SEARCH_FOR_PTHREAD()


####################################
### Main Library
####################################
SET(LIBRARY_NAME ${PROJECT_NAME})

# Verbosity level
IF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))
  ADD_DEFINITIONS(-DVP_DEBUG_MODE=${CMAKE_VERBOSITY_LEVEL} -DVP_DEBUG)
ENDIF (NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))

# Declare boost include directories
#include_directories(${Boost_INCLUDE_DIRS})
#link_directories(${Boost_LIBRARY_DIRS})

ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  src/debug/debug.cpp
  src/debug/real-time-logger.cpp
  src/debug/logger.cpp

  src/dgraph/entity.cpp
  src/dgraph/factory.cpp
  src/dgraph/pool.cpp

  src/exception/exception-abstract.cpp
  src/exception/exception-factory.cpp
  src/exception/exception-signal.cpp
  src/exception/exception-traces.cpp

  src/mt/process-list.cpp

  src/signal/signal-array.cpp
  src/signal/signal-caster.cpp
  src/signal/signal-cast-helper.cpp

  src/command/value.cpp
  src/command/command.cpp

  )

SET(${PROJECT_NAME}_HEADERS
  include/dynamic-graph/fwd.hh
  include/dynamic-graph/null-ptr.hh
  include/dynamic-graph/debug.h
  include/dynamic-graph/real-time-logger.h

  include/dynamic-graph/dynamic-graph-api.h

  include/dynamic-graph/entity.h
  include/dynamic-graph/factory.h
  include/dynamic-graph/pool.h

  include/dynamic-graph/exception-abstract.h
  include/dynamic-graph/exception-factory.h
  include/dynamic-graph/exception-signal.h
  include/dynamic-graph/exception-traces.h

  include/dynamic-graph/signal.h
  include/dynamic-graph/signal-array.h
  include/dynamic-graph/signal-base.h
  include/dynamic-graph/signal-ptr.h
  include/dynamic-graph/signal-time-dependent.h
  include/dynamic-graph/signal-ptr.t.cpp
  include/dynamic-graph/signal.t.cpp
  include/dynamic-graph/time-dependency.h
  include/dynamic-graph/time-dependency.t.cpp
  include/dynamic-graph/signal-caster.h
  include/dynamic-graph/signal-cast-helper.h
  include/dynamic-graph/all-signals.h
  include/dynamic-graph/signal-helper.h
  include/dynamic-graph/entity-helper.h

  include/dynamic-graph/tracer.h
  include/dynamic-graph/tracer-real-time.h

  include/dynamic-graph/command.h
  include/dynamic-graph/eigen-io.h
  include/dynamic-graph/linear-algebra.h
  include/dynamic-graph/value.h

  include/dynamic-graph/command-setter.h
  include/dynamic-graph/command-setter.t.cpp
  include/dynamic-graph/command-getter.h
  include/dynamic-graph/command-getter.t.cpp
  include/dynamic-graph/command-direct-getter.h
  include/dynamic-graph/command-direct-setter.h
  include/dynamic-graph/command-bind.h
  include/dynamic-graph/all-commands.h

  include/dynamic-graph/logger.h
)

SET(PUBLIC_HEADER ${${PROJECT_NAME}_HEADERS})

INSTALL(TARGETS ${PROJECT_NAME}
  EXPORT ${TARGETS_EXPORT_NAME}
  PUBLIC_HEADER
  INCLUDES DESTINATION include
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

# We need to include Eigen3 because the CMakeFile at 16.04 LTS
# is not so modern.
target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_BUILD_DIR}/include>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
   $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>
   INTERFACE ${EIGEN3_INCLUDE_DIR}
   $<INSTALL_INTERFACE:include>
   )

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})

IF (UNIX)
  TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${CMAKE_DL_LIBS} pthread)
ENDIF (UNIX)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${Boost_LIBRARIES})

####################################
### Plugins
####################################
SET(plugins_list
        src/traces/tracer
        src/traces/tracer-real-time
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

  INSTALL(TARGETS ${plugin}
    EXPORT ${TARGET_NAME}
    DESTINATION ${PLUGINDIR})
ENDFOREACH(plugin_file)

ADD_SUBDIRECTORY(include/${PROJECT_NAME})
ADD_SUBDIRECTORY(tests)


SETUP_PROJECT_PACKAGE_FINALIZE()

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

MESSAGE(STATUS "dynamic_graph_VERSION=${dynamic_graph_VERSION}")
