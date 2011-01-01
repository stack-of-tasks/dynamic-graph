#! /bin/sh
tmp=`mktemp`
echo "-- Launching dynamic-graph C shell with default plugins"
echo "loadPlugin ${CMAKE_INSTALL_PREFIX}/lib/plugin/shell-functions.so" > $tmp
echo "loadPlugin ${CMAKE_INSTALL_PREFIX}/lib/plugin/shell-procedure.so" >> $tmp
${CMAKE_INSTALL_PREFIX}/bin/dg-shell -i $tmp $@
