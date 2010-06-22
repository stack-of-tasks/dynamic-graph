#
#  Copyright
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECT_NAME dynamic-graph)
project(${PROJECT_NAME})

# --- PROJECT VERSION -----------------------
SET(${PROJECT_NAME}_VERSION_MAJOR "1")
SET(${PROJECT_NAME}_VERSION_MINOR "0")
SET(${PROJECT_NAME}_VERSION_PATCH "0")
SET(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")
SET(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})

SET(CMAKE_VERBOSE_MAKEFILE true)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# Get the OS
SET(OS ${CMAKE_SYSTEM_NAME})

#----------------------------------------------------
# --- GENERIC FLAGS ---------------------------------
#----------------------------------------------------
# Specify the compile flags for the overall library.
IF(WIN32)
  SET(LIBDIR_KW "/LIBPATH:")
  SET(LIBINCL_KW "")
  SET(LIB_EXT ".lib")
ENDIF(WIN32)

IF(UNIX)
  SET(LIBDIR_KW "-L")
  SET(LIBINCL_KW "-l")
  SET(LIB_EXT "")
ENDIF(UNIX)

# Link flags
SET(${PROJECT_NAME}_LINK_SRC_FLAGS "")
SET(${PROJECT_NAME}_LINK_FLAGS "${LIBDIR_KW}${${PROJECT_NAME}_BINARY_DIR}/lib ")
SET(${PROJECT_NAME}_LINK_FLAGS "${${PROJECT_NAME}_LINK_FLAGS} ${LIBINCLUSION_KW}${${PROJECT_NAME}_INTERN_LIBS}${LIBINCLUSION_EXT}")
IF(UNIX)
  SET(${PROJECT_NAME}_LINK_SRC_FLAGS "${${PROJECT_NAME}_LINK_SRC_FLAGS} -ldl")
  SET(${PROJECT_NAME}_LINK_FLAGS "${${PROJECT_NAME}_LINK_FLAGS} -ldl")
ENDIF(UNIX)

# Release flags
IF(\"${CMAKE_BUILD_TYPE}\" STREQUAL \"\" )
ELSE(\"${CMAKE_BUILD_TYPE}\" STREQUAL \"\" )
  IF(${CMAKE_BUILD_TYPE} STREQUAL DEBUG)
    SET(${PROJECT_NAME}_CXXFLAGS ${CMAKE_CXX_FLAGS_DEBUG}) # Useless since CXX_FLAGS, not CXXFLAGS.
  ENDIF(${CMAKE_BUILD_TYPE} STREQUAL DEBUG)
  IF(${CMAKE_BUILD_TYPE} STREQUAL RELEASE)
    SET(${PROJECT_NAME}_CXXFLAGS ${CMAKE_CXX_FLAGS_RELEASE}) # Same here.
  ENDIF(${CMAKE_BUILD_TYPE} STREQUAL RELEASE)
ENDIF (\"${CMAKE_BUILD_TYPE}\" STREQUAL \"\")

# --- Default options -------------------------------------------------
OPTION(BUILD_SHARED_LIBS "Build ${PROJECT_NAME} with shared libraries." ON)
OPTION(GENERATE_DOC "Generate html documentation" OFF)
OPTION(USE_BOOST "Compile with boost." ON)

#
# Look for dependencies
#

# Load Pkgconfig macros
INCLUDE(FindPkgConfig)
SET(PACKAGE_REQUIREMENTS "")

#
# pkg-config file
#

SET(install_pkg_prefix "\${prefix}")
SET(install_pkg_exec_prefix "\${exec_prefix}")
SET(install_pkg_libdir "\${libdir}")
SET(install_pkg_datarootdir "\${datarootdir}")
SET(install_pkg_include_dir "\${includedir}")

IF(UNIX)
  SET (${PROJECT_NAME}_LDFLAGS "-Wl,-R${install_pkg_libdir} ${LIBINCL_KW}${PROJECT_NAME}${LIB_EXT} ${${PROJECT_NAME}_LDFLAGS}")
ENDIF(UNIX)

IF(WIN32)
  SET (${PROJECT_NAME}_LDFLAGS "${LIBINCL_KW}${PROJECT_NAME}${LIB_EXT} ${${PROJECT_NAME}_LDFLAGS}")
ENDIF(WIN32)

# Install the file in the appropriate place.
SET(install_libdir_pkgconfig ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/dynamic-graph.pc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/dynamic-graph.pc)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/dynamic-graph.pc
  DESTINATION ${install_libdir_pkgconfig}	
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)

ENABLE_TESTING()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(tools)
ADD_SUBDIRECTORY(unitTesting)

#
# Documentation produced by doxygen
#
IF(GENERATE_DOC)
  INCLUDE(FindDoxygen)
  IF(DOXYGEN_FOUND)
    MESSAGE(STATUS "Doxygen found")
    add_subdirectory(doc)
  ENDIF(DOXYGEN_FOUND)
ENDIF(GENERATE_DOC)
