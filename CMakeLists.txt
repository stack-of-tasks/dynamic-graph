#
#  Copyright
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECT_NAME dynamicGraph)
project(${PROJECT_NAME})

SET(PROJECT_VERSION 1.0)

SET(CMAKE_VERBOSE_MAKEFILE true)

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

#
# Look for dependencies
#

# Load Pkgconfig macros
INCLUDE(FindPkgConfig)
SET(PACKAGE_REQUIREMENTS "")

#
#  Dependency to package MatrixAbstractLayer
#
SET(_MatrixAbstractLayer_FOUND 0)
SET(_MatrixAbstractLayer_REQUIRED "MatrixAbstractLayer >= 1.0")
PKG_CHECK_MODULES(_MatrixAbstractLayer REQUIRED ${_MatrixAbstractLayer_REQUIRED})
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS  --variable=docdir MatrixAbstractLayer
  OUTPUT_VARIABLE _MatrixAbstractLayer_DOCDIR)
MESSAGE(STATUS "pkg-config: ${PKG_CONFIG_EXECUTABLE}")
MESSAGE(STATUS "_MatrixAbstractLayer_DOCDIR=${_MatrixAbstractLayer_DOCDIR}")
SET(_MatrixAbstractLayer_FOUND 1)

IF(NOT ${_MatrixAbstractLayer_FOUND})
  MESSAGE(FATAL_ERROR "Check that package MatrixAbstractLayer is installed in a directory pointed out by PKG_CONFIG_PATH.")
ENDIF(NOT ${_MatrixAbstractLayer_FOUND})
  
IF(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS ${_MatrixAbstractLayer_REQUIRED})
ELSE(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS "${PACKAGE_REQUIREMENTS}, ${_MatrixAbstractLayer_REQUIRED}")
ENDIF(PACKAGE_REQUIREMENTS STREQUAL "")


#
#  Dependency to package dynamicsJRLJapan
#
SET(_dynamicsJRLJapan_FOUND 0)
SET(_dynamicsJRLJapan_REQUIRED "dynamicsJRLJapan >= 1.0")
PKG_CHECK_MODULES(_dynamicsJRLJapan REQUIRED ${_dynamicsJRLJapan_REQUIRED})
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS  --variable=docdir dynamicsJRLJapan
  OUTPUT_VARIABLE _dynamicsJRLJapan_DOCDIR)
MESSAGE(STATUS "pkg-config: ${PKG_CONFIG_EXECUTABLE}")
MESSAGE(STATUS "_dynamicsJRLJapan_DOCDIR=${_dynamicsJRLJapan_DOCDIR}")
SET(_dynamicsJRLJapan_FOUND 1)

IF(NOT ${_dynamicsJRLJapan_FOUND})
  MESSAGE(FATAL_ERROR "Check that package dynamicsJRLJapan is installed in a directory pointed out by PKG_CONFIG_PATH.")
ENDIF(NOT ${_dynamicsJRLJapan_FOUND})
  
IF(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS ${_dynamicsJRLJapan_REQUIRED})
ELSE(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS "${PACKAGE_REQUIREMENTS}, ${_dynamicsJRLJapan_REQUIRED}")
ENDIF(PACKAGE_REQUIREMENTS STREQUAL "")


#
#  Dependency to package hrp2Dynamics
#
SET(_hrp2Dynamics_FOUND 0)
SET(_hrp2Dynamics_REQUIRED "hrp2Dynamics >= 1.0")
PKG_CHECK_MODULES(_hrp2Dynamics REQUIRED ${_hrp2Dynamics_REQUIRED})
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS  --variable=docdir hrp2Dynamics
  OUTPUT_VARIABLE _hrp2Dynamics_DOCDIR)
MESSAGE(STATUS "pkg-config: ${PKG_CONFIG_EXECUTABLE}")
MESSAGE(STATUS "_hrp2Dynamics_DOCDIR=${_hrp2Dynamics_DOCDIR}")
SET(_hrp2Dynamics_FOUND 1)

IF(NOT ${_hrp2Dynamics_FOUND})
  MESSAGE(FATAL_ERROR "Check that package hrp2Dynamics is installed in a directory pointed out by PKG_CONFIG_PATH.")
ENDIF(NOT ${_hrp2Dynamics_FOUND})
  
IF(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS ${_hrp2Dynamics_REQUIRED})
ELSE(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS "${PACKAGE_REQUIREMENTS}, ${_hrp2Dynamics_REQUIRED}")
ENDIF(PACKAGE_REQUIREMENTS STREQUAL "")


#
#  Dependency to package walkGenJrl
#
SET(_walkGenJrl_FOUND 0)
SET(_walkGenJrl_REQUIRED "walkGenJrl >= 1.0")
PKG_CHECK_MODULES(_walkGenJrl REQUIRED ${_walkGenJrl_REQUIRED})
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS  --variable=docdir walkGenJrl
  OUTPUT_VARIABLE _walkGenJrl_DOCDIR)
MESSAGE(STATUS "pkg-config: ${PKG_CONFIG_EXECUTABLE}")
MESSAGE(STATUS "_walkGenJrl_DOCDIR=${_walkGenJrl_DOCDIR}")
SET(_walkGenJrl_FOUND 1)

IF(NOT ${_walkGenJrl_FOUND})
  MESSAGE(FATAL_ERROR "Check that package walkGenJrl is installed in a directory pointed out by PKG_CONFIG_PATH.")
ENDIF(NOT ${_walkGenJrl_FOUND})
  
IF(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS ${_walkGenJrl_REQUIRED})
ELSE(PACKAGE_REQUIREMENTS STREQUAL "")
  SET(PACKAGE_REQUIREMENTS "${PACKAGE_REQUIREMENTS}, ${_walkGenJrl_REQUIRED}")
ENDIF(PACKAGE_REQUIREMENTS STREQUAL "")

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

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  DESTINATION ${install_libdir_pkgconfig}	
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
#ADD_SUBDIRECTORY(unitTesting)

#
# Documentation produced by doxygen
#
INCLUDE(FindDoxygen)
IF(DOXYGEN_FOUND)
  add_subdirectory(doc)
ENDIF(DOXYGEN_FOUND)
