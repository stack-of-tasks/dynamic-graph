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

SET(tools dg-shell)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Configure shell launch script.
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/dg-shell-plugin.cmake
	       ${${PROJECT_NAME}_BINARY_DIR}/tools/dg-shell-plugin)
INSTALL(
	FILES
		${${PROJECT_NAME}_BINARY_DIR}/tools/dg-shell-plugin
	DESTINATION
		 bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                      GROUP_EXECUTE GROUP_READ)

FOREACH(tool_name ${tools})
	SET(EXECUTABLE_NAME ${tool_name})

	ADD_DEFINITIONS(-DDEBUG=2)

	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${tool_name}.cpp)

	INCLUDE_DIRECTORIES(
		${CMAKE_CURRENT_SOURCE_DIR}/../include
	)

	LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/lib)

	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  ${PROJECT_NAME})

	IF(UNIX)
		TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} dl)
	ENDIF(UNIX)

	INSTALL(TARGETS ${tool_name} DESTINATION bin)
ENDFOREACH(tool_name)
