set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

set(plugins traces/tracer traces/tracer-real-time)

set(tracer-real-time_deps tracer)

foreach(plugin ${plugins})
  get_filename_component(LIBRARY_NAME ${plugin} NAME)
  add_library(${LIBRARY_NAME} SHARED "${plugin}.cpp")

  if(SUFFIX_SO_VERSION)
    set_target_properties(${LIBRARY_NAME} PROPERTIES SOVERSION
                                                     ${PROJECT_VERSION})
  endif(SUFFIX_SO_VERSION)
  set_target_properties(${LIBRARY_NAME} PROPERTIES INSTALL_RPATH $ORIGIN)

  target_link_libraries(${LIBRARY_NAME} PUBLIC ${PROJECT_NAME}
                                               ${${LIBRARY_NAME}_deps})

  install(
    TARGETS ${LIBRARY_NAME}
    EXPORT ${TARGETS_EXPORT_NAME}
    DESTINATION ${DYNAMIC_GRAPH_PLUGINDIR})
endforeach(plugin)
