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

ADD_DEFINITIONS(-DDEBUG=2)

# Add Boost path to include directories.
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Make Boost.Test generates the main function in test cases.
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

# DYNAMIC_GRAPH_TEST(NAME)
# ------------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cpp', link it against
# Boost and add it to the test suite.
#
MACRO(DYNAMIC_GRAPH_TEST NAME)
  ADD_EXECUTABLE(${NAME} ${NAME}.cpp)
  ADD_TEST(${NAME} ${RUNTIME_OUTPUT_DIRECTORY}/${NAME})

  TARGET_LINK_LIBRARIES(${NAME} ${PROJECT_NAME})
  ADD_DEPENDENCIES(${NAME} ${PROJECT_NAME})

  # Link against Boost.
  TARGET_LINK_LIBRARIES(${NAME} ${Boost_LIBRARIES})
ENDMACRO(DYNAMIC_GRAPH_TEST)
DYNAMIC_GRAPH_TEST(test_depend)


# Signal cast test.
SET(signalcast_libs test_signalcast_libA test_signalcast_libB)

FOREACH(lib ${signalcast_libs})
  ADD_LIBRARY(${lib} SHARED ${lib}.cpp)

  TARGET_LINK_LIBRARIES(${lib} ${PROJECT_NAME})
  ADD_DEPENDENCIES(${lib} ${PROJECT_NAME})
ENDFOREACH()
DYNAMIC_GRAPH_TEST(test_signalcast)
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${signalcast_libs})

# Unit testing.
DYNAMIC_GRAPH_TEST(entity)
DYNAMIC_GRAPH_TEST(custom-entity)
DYNAMIC_GRAPH_TEST(factory)
DYNAMIC_GRAPH_TEST(pool)