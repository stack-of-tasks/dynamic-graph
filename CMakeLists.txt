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

SET(PROJECT_NAME dynamic-graph)
SET(PROJECT_VERSION 1.0.0.99)
SET(PROJECT_DESCRIPTION "Dynamic graph library")
SET(PROJECT_URL "http://github.com/jrl-umi3218/dynamic-graph")

# Defines paths.
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Disable -Werror on Unix for now.
SET(CXX_DISABLE_WERROR True)

SETUP_PROJECT()

# FIXME: to be changed into lib/dynamic-graph
# to avoid name collision when installing dynamic-graph in /usr.
SET(PLUGINDIR "lib/plugin")

# Add extra keys to pkg-config file to export the location of the
# plug-in directory in a robust manner.
SET(PKG_CONFIG_EXTRA
  "plugindirname=${PLUGINDIR}
plugindir=${CMAKE_INSTALL_PREFIX}/${PLUGINDIR}")

# Add dependency toward dynamic graph library in pkg-config file.
SET(PKG_CONFIG_LIBS "${PKG_CONFIG_LIBS} ${LIBINCL_KW}dynamic-graph${LIB_EXT}")

# Search for Boost.
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.33 COMPONENTS thread REQUIRED)
SET(PKG_CONFIG_CFLAGS "${PKG_CONFIG_CFLAGS} -I${Boost_INCLUDE_DIR}")
SET(PKG_CONFIG_LIBS "${PKG_CONFIG_LIBS} ${LIBDIR_KW}${Boost_LIBRARY_DIRS}")

ADD_DEFINITIONS(-DHAVE_LIBBOOST_THREAD)

# Search for Pthread.
INCLUDE(FindThreads)
IF(UNIX)
  IF(CMAKE_USE_PTHREADS_INIT)
    ADD_DEFINITIONS(-pthread)
  ELSE(CMAKE_USE_PTHREADS_INIT)
    MESSAGE(FATAL_ERROR
      "Pthread is required on Unix, but "
      ${CMAKE_THREAD_LIBS_INIT} " has been detected.")
  ENDIF(CMAKE_USE_PTHREADS_INIT)
ELSEIF(WIN32)
  # Nothing to do.
ELSE(UNIX)
  MESSAGE(FATAL_ERROR "Thread support for this platform is not implemented.")
ENDIF(UNIX)

# Remove additional warnings.
IF(WIN32)
  ADD_DEFINITIONS("/wd4251 /wd4275 /wd4101 /wd4355")
ENDIF(WIN32)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(unitTesting)

SETUP_PROJECT_FINALIZE()
