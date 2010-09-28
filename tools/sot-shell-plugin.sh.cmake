#! /bin/sh
echo "-- Launching dynamic-graph C shell with default plugins"
echo "loadPlugin ${CMAKE_INSTALL_PREFIX}/lib/plugin/shell-functions.so" > /tmp/default_script
echo "loadPlugin ${CMAKE_INSTALL_PREFIX}/lib/plugin/shell-procedure.so" >> /tmp/default_script
${CMAKE_INSTALL_PREFIX}/bin/sot-shell /tmp/default_script
