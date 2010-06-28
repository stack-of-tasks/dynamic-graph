echo "-- Launching dynamic-graph C shell with default plugins"
echo "loadPlugin ${CMAKE_INSTALL_PREFIX}/lib/plugin/shell-functions.so" > ./default_script
echo "loadPlugin ${CMAKE_INSTALL_PREFIX}/lib/plugin/shell-procedure.so" >> ./default_script
${CMAKE_INSTALL_PREFIX}/bin/${PROJECT_NAME}/test_shell default_script