cmake_minimum_required(VERSION 3.1)

# Project properties
set(PROJECT_ORG stack-of-tasks)
set(PROJECT_NAME dynamic-graph)
set(PROJECT_DESCRIPTION "Dynamic graph library")
set(PROJECT_URL "https://github.com/${PROJECT_ORG}/${PROJECT_NAME}")

# Project options
option(SUFFIX_SO_VERSION "Suffix library name with its version" ON)

# Project configuration
set(PROJECT_USE_CMAKE_EXPORT TRUE)
set(CUSTOM_HEADER_DIR ${PROJECT_NAME})
set(CXX_DISABLE_WERROR TRUE)

# Doxygen setup
set(DOXYGEN_USE_MATHJAX YES)
set(DOXYGEN_USE_TEMPLATE_CSS YES)

# JRL-cmakemodule setup
include(cmake/base.cmake)
include(cmake/boost.cmake)

# Project definition
compute_project_args(PROJECT_ARGS LANGUAGES CXX)
project(${PROJECT_NAME} ${PROJECT_ARGS})
check_minimal_cxx_standard(14 ENFORCE)

include(cmake/pthread.cmake) # needs to be included after the CXX definition

# Project dependencies
if(BUILD_TESTING)
  find_package(Boost REQUIRED COMPONENTS unit_test_framework)
endif()
add_project_dependency(Boost REQUIRED COMPONENTS serialization system thread)
add_project_dependency(Eigen3 REQUIRED)
search_for_pthread()

if(Boost_VERSION GREATER 107299 OR Boost_VERSION_MACRO GREATER 107299)
  # Silence a warning about a deprecated use of boost bind by boost >= 1.73
  # without dropping support for boost < 1.73
  add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
endif()

# Add configuration headers for plug-ins.
generate_configuration_header(${HEADER_DIR} config-tracer.hh DG_TRACER
                              tracer_EXPORTS)
generate_configuration_header(${HEADER_DIR} config-tracer-real-time.hh
                              DG_TRACERREALTIME tracer_real_time_EXPORTS)

# Verbosity level
if(NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))
  add_definitions(-DVP_DEBUG_MODE=${CMAKE_VERBOSITY_LEVEL} -DVP_DEBUG)
endif(NOT (\"${CMAKE_VERBOSITY_LEVEL}\" STREQUAL \"\"))

# Main Library
set(${PROJECT_NAME}_HEADERS
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
    include/${CUSTOM_HEADER_DIR}/logger.h)

set(${PROJECT_NAME}_SOURCES
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
    src/command/command.cpp)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES}
                                   ${${PROJECT_NAME}_HEADERS})
modernize_target_link_libraries(
  ${PROJECT_NAME}
  SCOPE
  PUBLIC
  TARGETS
  Eigen3::Eigen
  INCLUDE_DIRS
  ${EIGEN3_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::serialization Boost::system
                                             Boost::thread)

if(UNIX)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS} pthread)
endif(UNIX)

if(SUFFIX_SO_VERSION)
  set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
endif(SUFFIX_SO_VERSION)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${TARGETS_EXPORT_NAME}
  DESTINATION lib)

set(DYNAMIC_GRAPH_PLUGINDIR "lib/${PROJECT_NAME}-plugins")
set(PACKAGE_EXTRA_MACROS
    "set(DYNAMIC_GRAPH_PLUGINDIR ${DYNAMIC_GRAPH_PLUGINDIR})")
set(PKG_CONFIG_EXTRA "plugindir=${DYNAMIC_GRAPH_PLUGINDIR}")

add_subdirectory(src)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif(BUILD_TESTING)

pkg_config_append_libs(${PROJECT_NAME})
install(FILES package.xml DESTINATION share/${PROJECT_NAME})
