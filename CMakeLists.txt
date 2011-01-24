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

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/pthread.cmake)
INCLUDE(cmake/cpack.cmake)

SET(PROJECT_NAME dynamic-graph)
SET(PROJECT_DESCRIPTION "Dynamic graph library")
SET(PROJECT_URL "http://github.com/jrl-umi3218/dynamic-graph")

SET(CUSTOM_HEADER_DIR "${PROJECT_NAME}")

# Defines paths.
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SETUP_PROJECT()

# Trigger dependency to jrl-mal
ADD_REQUIRED_DEPENDENCY("jrl-mal >= 1.8.0")

# Add configuration headers for plug-ins.
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR} config-tracer.hh DG_TRACER tracer_EXPORTS)
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR}
  config-tracer-real-time.hh DG_TRACERREALTIME tracer_real_time_EXPORTS)
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR}
  config-shell-procedure.hh DG_SHELLPROCEDURE shell_procedure_EXPORTS)
GENERATE_CONFIGURATION_HEADER(
  ${HEADER_DIR}
  config-contiifstream.hh DG_CONTIIFSTREAM contiifstream_EXPORTS)

# FIXME: to be changed into lib/dynamic-graph
# to avoid name collision when installing dynamic-graph in /usr.
SET(PLUGINDIR "lib/plugin")

# Add extra keys to pkg-config file to export the location of the
# plug-in directory in a robust manner.
SET(PKG_CONFIG_EXTRA
  "plugindirname=${PLUGINDIR}
plugindir=${CMAKE_INSTALL_PREFIX}/${PLUGINDIR}")

# Add dependency toward dynamic graph library in pkg-config file.
PKG_CONFIG_APPEND_LIBS("dynamic-graph")

# Search for Boost.
SEARCH_FOR_BOOST()
SEARCH_FOR_PTHREAD()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(share)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()