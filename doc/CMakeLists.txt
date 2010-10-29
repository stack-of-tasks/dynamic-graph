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

INCLUDE(../cmake/man.cmake)

IF(UNIX)
  # Add `man' target.
  ADD_CUSTOM_TARGET(
    man ALL
    DEPENDS dg-shell.1.gz dg-shell-plugin.1.gz)

  # Man page generation.
  MANPAGE(dg-shell)
  MANPAGE(dg-shell-plugin)
ENDIF(UNIX)
