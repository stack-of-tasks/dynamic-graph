CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

# Project properties
SET(PROJECT_ORG stack-of-tasks)
SET(PROJECT_NAME dynamic-graph)
SET(PROJECT_DESCRIPTION "Dynamic graph library")
SET(PROJECT_URL "https://github.com/${PROJECT_ORG}/${PROJECT_NAME}")
SET(PROJECT_SUFFIX "-v3")

# Project options
OPTION(SUFFIX_SO_VERSION "Suffix library name with its version" ON)

# Project configuration
SET(PROJECT_USE_CMAKE_EXPORT TRUE)
SET(CUSTOM_HEADER_DIR ${PROJECT_NAME})
SET(CXX_DISABLE_WERROR TRUE)

# Doxygen setup
SET(DOXYGEN_USE_MATHJAX YES)
SET(DOXYGEN_USE_TEMPLATE_CSS YES)

# JRL-cmakemodule setup
INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)

# Project definition
COMPUTE_PROJECT_ARGS(PROJECT_ARGS LANGUAGES CXX)
PROJECT(${PROJECT_NAME} ${PROJECT_ARGS})
CHECK_MINIMAL_CXX_STANDARD(11 ENFORCE)

INCLUDE(cmake/pthread.cmake)  # needs to be included after the CXX definition

# Project dependencies
FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)
ADD_PROJECT_DEPENDENCY(Boost REQUIRED COMPONENTS serialization system thread)
ADD_PROJECT_DEPENDENCY(Eigen3 REQUIRED PKG_CONFIG_REQUIRES eigen3)
SEARCH_FOR_PTHREAD()

# Add configuration headers for plug-ins.
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR} config-tracer.hh DG_TRACER tracer_EXPORTS)
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR}
  config-tracer-real-time.hh DG_TRACERREALTIME tracer_real_time_EXPORTS)

# Verbosity level
IF(NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))
  ADD_DEFINITIONS(-DVP_DEBUG_MODE=${CMAKE_VERBOSITY_LEVEL} -DVP_DEBUG)
ENDIF(NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))

# Main Library
SET(${PROJECT_NAME}_HEADERS
  include/${CUSTOM_HEADER_DIR}/fwd.hh
  include/${CUSTOM_HEADER_DIR}/debug.h
  include/${CUSTOM_HEADER_DIR}/real-time-logger.h
  include/${CUSTOM_HEADER_DIR}/real-time-logger-def.h

  include/${CUSTOM_HEADER_DIR}/dynamic-graph-api.h

  include/${CUSTOM_HEADER_DIR}/entity.h
  include/${CUSTOM_HEADER_DIR}/factory.h
  include/${CUSTOM_HEADER_DIR}/pool.h

  include/${CUSTOM_HEADER_DIR}/exception-abstract.h
  include/${CUSTOM_HEADER_DIR}/exception-factory.h
  include/${CUSTOM_HEADER_DIR}/exception-signal.h
  include/${CUSTOM_HEADER_DIR}/exception-traces.h

  include/${CUSTOM_HEADER_DIR}/signal.h
  include/${CUSTOM_HEADER_DIR}/signal-array.h
  include/${CUSTOM_HEADER_DIR}/signal-base.h
  include/${CUSTOM_HEADER_DIR}/signal-ptr.h
  include/${CUSTOM_HEADER_DIR}/signal-time-dependent.h
  include/${CUSTOM_HEADER_DIR}/signal-ptr.t.cpp
  include/${CUSTOM_HEADER_DIR}/signal.t.cpp
  include/${CUSTOM_HEADER_DIR}/time-dependency.h
  include/${CUSTOM_HEADER_DIR}/time-dependency.t.cpp
  # Kept for a brittle backward compatiblity.
  include/${CUSTOM_HEADER_DIR}/signal-caster.h
  include/${CUSTOM_HEADER_DIR}/signal-cast-helper.h
  include/${CUSTOM_HEADER_DIR}/all-signals.h
  include/${CUSTOM_HEADER_DIR}/signal-helper.h
  include/${CUSTOM_HEADER_DIR}/entity-helper.h

  include/${CUSTOM_HEADER_DIR}/tracer.h
  include/${CUSTOM_HEADER_DIR}/tracer-real-time.h

  include/${CUSTOM_HEADER_DIR}/command.h
  include/${CUSTOM_HEADER_DIR}/eigen-io.h
  include/${CUSTOM_HEADER_DIR}/linear-algebra.h
  include/${CUSTOM_HEADER_DIR}/value.h

  include/${CUSTOM_HEADER_DIR}/command-setter.h
  include/${CUSTOM_HEADER_DIR}/command-setter.t.cpp
  include/${CUSTOM_HEADER_DIR}/command-getter.h
  include/${CUSTOM_HEADER_DIR}/command-getter.t.cpp
  include/${CUSTOM_HEADER_DIR}/command-direct-getter.h
  include/${CUSTOM_HEADER_DIR}/command-direct-setter.h
  include/${CUSTOM_HEADER_DIR}/command-bind.h
  include/${CUSTOM_HEADER_DIR}/all-commands.h

  include/${CUSTOM_HEADER_DIR}/logger.h
  )

SET(${PROJECT_NAME}_SOURCES
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

  src/command/value.cpp
  src/command/command.cpp
  )

ADD_LIBRARY(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:include>)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

IF(UNIX)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS} pthread)
ENDIF(UNIX)

IF(SUFFIX_SO_VERSION)
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
ENDIF(SUFFIX_SO_VERSION)

INSTALL(TARGETS ${PROJECT_NAME} EXPORT ${TARGETS_EXPORT_NAME} DESTINATION lib)


SET(DYNAMIC_GRAPH_PLUGINDIR "${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME}-plugins")
SET(PACKAGE_EXTRA_MACROS "set(DYNAMIC_GRAPH_PLUGINDIR ${DYNAMIC_GRAPH_PLUGINDIR})")
SET(PKG_CONFIG_EXTRA "plugindir=${DYNAMIC_GRAPH_PLUGINDIR}")
SET(CMAKE_INSTALL_RPATH "${DYNAMIC_GRAPH_PLUGINDIR}")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)

PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})
INSTALL(FILES package.xml DESTINATION share/${PROJECT_NAME})
