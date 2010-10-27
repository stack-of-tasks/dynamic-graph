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


SET(BUILD_DIR ${CMAKE_BINARY_DIR}/doc)
SET(DOCDIR share/doc/${PROJECT_NAME})
SET(HTMLDIR ${DOCDIR}/doxygen-html)

# MANPAGE
# -------
#
# Generate a pod man page from a template, then
# generate the man page and compress it.
# This macro also adds the installation rules.
#
MACRO(MANPAGE NAME)
  FIND_PROGRAM(POD2MAN pod2man)
  CONFIGURE_FILE(${NAME}.pod.in ${BUILD_DIR}/${NAME}.pod @ONLY)

  ADD_CUSTOM_COMMAND(
    OUTPUT ${NAME}.1
    COMMAND ${POD2MAN} --section=1
            --center="LOCAL USER COMMANDS"
	    --release ${PROJECT_NAME} ${NAME}.pod
	    > ${NAME}.1
    DEPENDS ${BUILD_DIR}/${NAME}.pod)

  ADD_CUSTOM_COMMAND(
    OUTPUT ${NAME}.1.gz
    COMMAND gzip -c ${NAME}.1 > ${NAME}.1.gz
    DEPENDS ${BUILD_DIR}/${NAME}.1)
  INSTALL(FILES ${BUILD_DIR}/${NAME}.1.gz DESTINATION share/man/man1)
ENDMACRO(MANPAGE)


# Generate Doxygen configuration file.
CONFIGURE_FILE(package.dox.cmake ${BUILD_DIR}/package.dox)


# Teach CMake how to generate the documentation.
ADD_CUSTOM_TARGET(doc
	COMMAND ${DOXYGEN_EXECUTABLE} package.dox
	WORKING_DIRECTORY doc
	DEPENDS ${BUILD_DIR}/dg-shell.1.gz ${BUILD_DIR}/dg-shell-plugin.1.gz
)

ADD_CUSTOM_COMMAND(
	OUTPUT
	${BUILD_DIR}/${PROJECT_NAME}.doxytag
	${BUILD_DIR}/doxygen-html
	COMMAND ${DOXYGEN_EXECUTABLE} package.dox
	WORKING_DIRECTORY doc
	COMMENT "Generating Doxygen documentation"
)

# Doxygen documentation installation rules.
  # Install generated files.
INSTALL(
	CODE "EXECUTE_PROCESS(COMMAND make doc)"
	FILES ${BUILD_DIR}/${PROJECT_NAME}.doxytag
	DESTINATION share/doc/${PROJECT_NAME}/doxygen-html)
INSTALL(DIRECTORY ${BUILD_DIR}/doxygen-html
	DESTINATION share/doc/${PROJECT_NAME})
INSTALL(DIRECTORY pictures DESTINATION ${HTMLDIR})


# Man page generation.
MANPAGE(dg-shell)
MANPAGE(dg-shell-plugin)
