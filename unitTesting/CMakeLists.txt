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

### tests
SET(tests_exe
	test_pool
	test_depend
	test_signalcast)

SET(tests_libs
	test_signalcast_libA
	test_signalcast_libB
)

SET(test_signalcast_additional_libs ${test_libs})

ADD_DEFINITIONS(-DDEBUG=2)

LINK_DIRECTORIES(../src)

# Additional tests
FOREACH(test_name ${tests_libs})
	SET(LIBRARY_NAME ${test_name})

	ADD_LIBRARY(${LIBRARY_NAME}
	  SHARED
	  ${test_name}.cpp)

	TARGET_LINK_LIBRARIES(${LIBRARY_NAME}
	  ${PROJECT_NAME})
ENDFOREACH(test_name)

FOREACH(test_name ${tests_exe})
	SET(EXECUTABLE_NAME ${test_name})

	ADD_EXECUTABLE(${EXECUTABLE_NAME}
	  ${test_name}.cpp)

	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
	  ${PROJECT_NAME})

	TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME}
		${${test_name}_additional_libs})

	ADD_TEST(NAME ${test_name} COMMAND ${test_name})
ENDFOREACH(test_name)
