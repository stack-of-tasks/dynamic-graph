# Copyright 2010, Olivier Stasse, JRL, CNRS/AIST

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


SET(PROJECT_ORG stack-of-tasks)
SET(PROJECT_NAME dynamic-graph)
SET(PROJECT_DESCRIPTION "Dynamic graph library")
SET(PROJECT_URL "http://github.com/${PROJECT_ORG}/${PROJECT_NAME}")
SET(PROJECT_SUFFIX "-v3")

INCLUDE(cmake/base.cmake)
PROJECT(${PROJECT_NAME} CXX)

INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/eigen.cmake)
INCLUDE(cmake/pthread.cmake)

# Export CMake Target
SET(PROJECT_USE_CMAKE_EXPORT TRUE)

SET(CUSTOM_HEADER_DIR "${PROJECT_NAME}")

SET(DOXYGEN_USE_MATHJAX YES)

# Defines paths.
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Add configuration headers for plug-ins.
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR} config-tracer.hh DG_TRACER tracer_EXPORTS)
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR}
  config-tracer-real-time.hh DG_TRACERREALTIME tracer_real_time_EXPORTS)

# FIXME: to be changed into lib/dynamic-graph
# to avoid name collision when installing dynamic-graph in /usr.
SET(PLUGINDIR "${CMAKE_INSTALL_FULL_LIBDIR}/plugin")

# Add extra keys to pkg-config file to export the location of the
# plug-in directory in a robust manner.
SET(PKG_CONFIG_EXTRA "plugindir=${PLUGINDIR}")

# Add dependency toward dynamic graph library in pkg-config file.
PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

# Search for Boost.
SET(BOOST_COMPONENTS serialization system unit_test_framework thread)
SEARCH_FOR_BOOST()
SEARCH_FOR_PTHREAD()

# Search for Boost.
SEARCH_FOR_EIGEN()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include/${PROJECT_NAME})
ADD_SUBDIRECTORY(tests)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_PACKAGE_FINALIZE()
