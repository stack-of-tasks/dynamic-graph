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

# Add default import directories.
#  It is used by the import directive of the sot shell to locate
#  scripts and plug-ins.
SET(
  DG_IMPORT_DEFAULT_PATHS
  "${CMAKE_INSTALL_PREFIX}/share/dynamic-graph/script:${CMAKE_INSTALL_PREFIX}/${PLUGINDIR}"
  )

CONFIGURE_FILE(
	${PROJECT_NAME}/import-default-paths.h.cmake	
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/import-default-paths.h)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/import-default-paths.h
       DESTINATION include/${PROJECT_NAME}
       PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE
)


# Headers list.
SET(${PROJECT_NAME}_HEADERS
fwd.hh
null-ptr.hh
contiifstream.h
debug.h

dynamic-graph-api.h

entity.h
factory.h
pool.h

exception-abstract.h
exception-factory.h
exception-signal.h
exception-traces.h

signal.h
signal-array.h
signal-base.h
signal-ptr.h
signal-time-dependent.h
signal-ptr.t.cpp
signal.t.cpp
time-dependency.h
time-dependency.t.cpp
signal-caster.h
signal-cast-helper.h
all-signals.h

tracer.h
tracer-real-time.h

command.h
linear-algebra.h
value.h

command-setter.h
command-setter.t.cpp
command-getter.h
command-getter.t.cpp
command-direct-getter.h
command-direct-setter.h
command-bind.h
all-commands.h
)

# Recreate correct path for the headers
#--------------------------------------

SET(fullpath_${PROJECT_NAME}_HEADERS)
FOREACH(lHeader ${${PROJECT_NAME}_HEADERS})
 SET(fullpath_${PROJECT_NAME}_HEADERS
   ${fullpath_${PROJECT_NAME}_HEADERS}
   ./${PROJECT_NAME}/${lHeader}
 )

ENDFOREACH(lHeader)

#----------------------------------------------------
# Install procedure for the header files
#----------------------------------------------------
INSTALL(FILES ${fullpath_${PROJECT_NAME}_HEADERS}
       DESTINATION include/${PROJECT_NAME}
       PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE
)
