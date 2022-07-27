# Copyright 2010-2020, Olivier Stasse, Guilhem Saurel, JRL, CNRS/AIST, LAAS-CNRS

add_definitions(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

add_definitions(-DTESTS_DATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DTESTS_PLUGINDIR="${LIBRARY_OUTPUT_PATH}")
add_definitions(-DTESTS_DYNLIBSUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}")

macro(DYNAMIC_GRAPH_TEST NAME)
  add_unit_test(${NAME} "${NAME}.cpp")
  target_link_libraries(${NAME} PRIVATE ${PROJECT_NAME}
                                        Boost::unit_test_framework)
endmacro(DYNAMIC_GRAPH_TEST)

# Signal cast test.
set(signalcast_libs signal-cast-registerer-libA signal-cast-registerer-libB)

foreach(lib ${signalcast_libs})
  add_library(${lib} SHARED "${lib}.cpp")
  target_link_libraries(${lib} PRIVATE ${PROJECT_NAME})
endforeach()

dynamic_graph_test(signal-cast-registerer)

# Unit testing.
if(NOT APPLE)
  dynamic_graph_test(entity)
endif(NOT APPLE)
dynamic_graph_test(custom-entity)
dynamic_graph_test(factory)
dynamic_graph_test(pool)
dynamic_graph_test(signal-time-dependent)
dynamic_graph_test(value)
dynamic_graph_test(signal-ptr)
dynamic_graph_test(real-time-logger)
dynamic_graph_test(debug-trace)
dynamic_graph_test(debug-tracer)
target_link_libraries(debug-tracer PRIVATE tracer)
dynamic_graph_test(debug-real-time-tracer)
target_link_libraries(debug-real-time-tracer PRIVATE tracer-real-time tracer)
dynamic_graph_test(debug-logger)
dynamic_graph_test(debug-logger-winit)
dynamic_graph_test(signal-all)
dynamic_graph_test(command-test)
dynamic_graph_test(test-mt)
target_link_libraries(test-mt PRIVATE tracer)
dynamic_graph_test(exceptions)
