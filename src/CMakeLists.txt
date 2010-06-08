# 
#  Copyright 
# 

# Create the library "dynamicGraph" 
SET(libdg_src "")
SET(libdg_headers "../include/dynamicGraph-API.h")
SET(libdg_templates "")
SET(libdg_plugins "")
SET(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIBRARY_NAME dynamicGraph)

#ordre est important
INCLUDE_DIRECTORIES(
${${PROJECT_NAME}_BINARY_DIR}/include
${CMAKE_SOURCE_DIR}/include
)

ADD_DEFINITIONS(-DDEBUG=2)

# provide path to library libMatrixAbstractLayer.so
LINK_DIRECTORIES(${_MatrixAbstractLayer_LIBRARY_DIRS})


# provide path to library libdynamicsJRLJapan.so
LINK_DIRECTORIES(${_dynamicsJRLJapan_LIBRARY_DIRS})


# provide path to library libhrp2Dynamics.so
LINK_DIRECTORIES(${_hrp2Dynamics_LIBRARY_DIRS})


# provide path to library libwalkGenJrl.so
LINK_DIRECTORIES(${_walkGenJrl_LIBRARY_DIRS})


#############################################"

MACRO(SOT_SRC_SUBDIR_PROCESS)
  # Guess the .h names from .cpp names.
  FOREACH(srcfile ${libdg_${SOT_MODULE_NAME}_src})
     SET(newsrcfile "")
     STRING(REGEX REPLACE "cpp$" "h" newsrcfile ${srcfile})
     LIST(APPEND libdg_${SOT_MODULE_NAME}_headers ${newsrcfile})
  ENDFOREACH(srcfile)
  # Same for plugins.
  FOREACH(srcfile ${libdg_${SOT_MODULE_NAME}_plugins_src})
     SET(newsrcfile "")
     STRING(REGEX REPLACE "cpp$" "h" newsrcfile ${srcfile})
     LIST(APPEND libdg_${SOT_MODULE_NAME}_plugins_headers ${newsrcfile})
  ENDFOREACH(srcfile)
  
  # Guess the .h names from .t.cpp template names.
  SET(tempext "t\\.cpp")
  FOREACH(srcfile ${libdg_${SOT_MODULE_NAME}_templates})
     SET(newsrcfile "")
     STRING(REGEX REPLACE "${tempext}$" "h" newsrcfile ${srcfile})
     LIST(APPEND libdg_${SOT_MODULE_NAME}_headers ${newsrcfile})
  ENDFOREACH(srcfile)
  # Same for plugins.
  FOREACH(srcfile ${libdg_${SOT_MODULE_NAME}_plugins_templates})
     LIST(APPEND libdg_${SOT_MODULE_NAME}_plugins_headers ${srcfile})
  ENDFOREACH(srcfile)
  
  # Down to the previous level.
  LIST(APPEND libdg_src ${libdg_${SOT_MODULE_NAME}_src})
  LIST(APPEND libdg_headers ${libdg_${SOT_MODULE_NAME}_headers})
  LIST(APPEND libdg_templates ${libdg_${SOT_MODULE_NAME}_templates})
  LIST(APPEND libdg_plugins_src ${libdg_${SOT_MODULE_NAME}_plugins_src})
  LIST(APPEND libdg_plugins_headers ${libdg_${SOT_MODULE_NAME}_plugins_headers})
  LIST(APPEND libdg_plugins_templates ${libdg_${SOT_MODULE_NAME}_plugins_templates})
ENDMACRO(SOT_SRC_SUBDIR_PROCESS)


INCLUDE(debug/SourcesLib.cmake)
INCLUDE(dgraph/SourcesLib.cmake)
INCLUDE(signal/SourcesLib.cmake)
INCLUDE(exception/SourcesLib.cmake)

# -------------------------------------------------------------
# --- HEADERS -------------------------------------------------
# -------------------------------------------------------------
# Create rule to copy all the headers form src to include/src
# For each header we create a rule
SET(HEADER_IN_INCLUDE_DIR "")
SET(ADDED_SRC_DEPEND_PROPERTIES "")
FOREACH (header ${libdg_headers} ${libdg_templates}) 
  GET_FILENAME_COMPONENT(headerName ${header} NAME)
  ADD_CUSTOM_COMMAND(
   OUTPUT ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${headerName}
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${header} 
           ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${headerName}
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${header}  
  )
  INSTALL(FILES
	   ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${headerName}
	   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/dynamicGraph
	   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
  SET(HEADER_IN_INCLUDE_DIR ${HEADER_IN_INCLUDE_DIR} 
       ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${headerName})
ENDFOREACH(header)

# --- Create a top level rule to copy all the headers from src to include/dynamicGraph
ADD_CUSTOM_TARGET(header ALL
		   DEPENDS ${HEADER_IN_INCLUDE_DIR})

# --- Dependencies of the dgraph library on the headers.
ADD_LIBRARY(${LIBRARY_NAME} 
  ${libdg_src} ${libdg_headers}
)

# Add MatrixAbstractLayer compilation flags and link to library libMatrixAbstractLayer.so
ADD_DEFINITIONS(${_MatrixAbstractLayer_CFLAGS})
IF(UNIX)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${_MatrixAbstractLayer_LIBRARIES})
ENDIF(UNIX)

IF(WIN32)
foreach(dlink ${_MatrixAbstractLayer_LDFLAGS})
	SET ( ${PROJECT_NAME}_src_LDFLAGS "${${PROJECT_NAME}_src_LDFLAGS}  ${dlink}")
endforeach(dlink)
ENDIF(WIN32)

# Add dynamicsJRLJapan compilation flags and link to library libdynamicsJRLJapan.so
ADD_DEFINITIONS(${_dynamicsJRLJapan_CFLAGS})
IF(UNIX)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${_dynamicsJRLJapan_LIBRARIES})
ENDIF(UNIX)

IF(WIN32)
foreach(dlink ${_dynamicsJRLJapan_LDFLAGS})
	SET ( ${PROJECT_NAME}_src_LDFLAGS "${${PROJECT_NAME}_src_LDFLAGS}  ${dlink}")
endforeach(dlink)
ENDIF(WIN32)

# Add hrp2Dynamics compilation flags and link to library libhrp2Dynamics.so
ADD_DEFINITIONS(${_hrp2Dynamics_CFLAGS})
IF(UNIX)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${_hrp2Dynamics_LIBRARIES})
ENDIF(UNIX)

IF(WIN32)
foreach(dlink ${_hrp2Dynamics_LDFLAGS})
	SET ( ${PROJECT_NAME}_src_LDFLAGS "${${PROJECT_NAME}_src_LDFLAGS}  ${dlink}")
endforeach(dlink)
ENDIF(WIN32)

# Add walkGenJrl compilation flags and link to library libwalkGenJrl.so
ADD_DEFINITIONS(${_walkGenJrl_CFLAGS})
IF(UNIX)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${_walkGenJrl_LIBRARIES})
ENDIF(UNIX)

IF(WIN32)
foreach(dlink ${_walkGenJrl_LDFLAGS})
	SET ( ${PROJECT_NAME}_src_LDFLAGS "${${PROJECT_NAME}_src_LDFLAGS}  ${dlink}")
endforeach(dlink)
ENDIF(WIN32)

# -------------------------------------------------------------
# --- LIBS ----------------------------------------------------
# -------------------------------------------------------------

SET_TARGET_PROPERTIES(${LIBRARY_NAME} 
  PROPERTIES 
  SOVERSION ${PROJECT_VERSION}
  INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

SET_TARGET_PROPERTIES(${LIBRARY_NAME} 
  PROPERTIES
  COMPILE_FLAGS "${${PROJECT_NAME}_src_CXXFLAGS}"
  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS}"
)

# --- Force the creation of the headers before compiling the lib.
ADD_DEPENDENCIES(${LIBRARY_NAME} header)

# Install the main lib.
INSTALL(TARGETS ${LIBRARY_NAME}
   DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

   
# -------------------------------------------------------------
# --- PLUGINS -------------------------------------------------
# -------------------------------------------------------------

# -------------------------------------------------------------------
# --- UTILS ---------------------------------------------------------
# -------------------------------------------------------------------
MACRO(LIST2STRING _VAR_ARG _LIST_ARG)
  SET(${_VAR_ARG})
  SET(_FIRST_STEP 1)
  FOREACH(_ITEM ${ARGV})
     IF(_FIRST_STEP)
	SET(_FIRST_STEP 0)
     ELSE(_FIRST_STEP)
           SET (${_VAR_ARG} "${${_VAR_ARG}} ${_ITEM}")
     ENDIF(_FIRST_STEP)
  ENDFOREACH(_ITEM)
ENDMACRO(LIST2STRING)

# Create rule to compile all the plugins form src to include/src.
# For each plugin we create a rule.
SET(PLUGINS_IN_LIBPATH "")
SET(srcextension "\\.cpp")
SET(srcheader "\\.h")
SET(srctemplate "t\\.cpp")
MAKE_DIRECTORY("${${PROJECT_NAME}_BINARY_DIR}/lib/plugin")

# --- PLUGIN HEADERs -----------------------------------------
# Add the command that will copy the header of each plugin 
#  in the include repository.
FOREACH (plugin ${libdg_plugins_headers}) 
  # Create the name of the library out of the name of the plugin.
  GET_FILENAME_COMPONENT(pluginHeader ${plugin} NAME)
  STRING(REGEX REPLACE "${srcheader}$" "" newsofile ${pluginHeader})
  
  # Add a command to copy the header.
  ADD_CUSTOM_COMMAND(
	OUTPUT ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginHeader}
	COMMAND ${CMAKE_COMMAND} -E copy 
	${CMAKE_CURRENT_SOURCE_DIR}/${plugin} 
	${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginHeader}
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${plugin})

  INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginHeader}
	   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/dynamicGraph
	   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
  
  # Create a top level rule to copy all the headers from src to include/dg
  ADD_CUSTOM_TARGET(header_${newsofile} ALL
	 DEPENDS ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginHeader})
ENDFOREACH(plugin)

# --- PLUGIN TEMPLATEs -----------------------------------------
# Add the command that will copy the header of each plugin 
#  in the include repository.
FOREACH (plugin ${libdg_plugins_templates}) 
  # Create the name of the library out of the name of the plugin.
  GET_FILENAME_COMPONENT(pluginTemplate ${plugin} NAME)
  STRING(REGEX REPLACE "${srctemplate}$" "" newsofile ${pluginTemplate})
  STRING(REGEX REPLACE ".${srctemplate}$" "" newsofile2 ${pluginTemplate})
  SET(${newsofile2}Template ${plugin})

  # Add a command to copy the header.
  ADD_CUSTOM_COMMAND(
	OUTPUT ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginTemplate}
	COMMAND ${CMAKE_COMMAND} -E copy 
	${CMAKE_CURRENT_SOURCE_DIR}/${plugin} 
	${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginTemplate}
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${plugin})

  INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginTemplate}
	   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/dynamicGraph
	   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
  
  # Create a top level rule to copy all the headers from src to include/dg
  ADD_CUSTOM_TARGET(header_${newsofile} ALL
	  	     DEPENDS ${${PROJECT_NAME}_BINARY_DIR}/include/dynamicGraph/${pluginTemplate})
ENDFOREACH(plugin)

# --- PLUGIN LINKS -------------------------------------------
# Specific LD Flags for plugins.
LIST(APPEND ${PROJECT_NAME}_PLUGIN_LDFLAGS ${${PROJECT_NAME}_LINK_SRC_FLAGS})

# --- PLUGIN COMPILATION -----------------------------------------------
FOREACH(plugin ${libdg_plugins_src}) 
  # Create the name of the library out of the name of the plugin.
  GET_FILENAME_COMPONENT(pluginName ${plugin} NAME)
  STRING(REGEX REPLACE "${srcextension}$" "" newsofile ${pluginName})
  STRING(REGEX REPLACE ${srcextension}$ .h pluginHeader ${plugin})

  # Specify the new target.
  SET(plugin_add_srcsname ${newsofile}_additional_srcs)
  SET(pluginTemplate ${newsofile}Template)

  ADD_LIBRARY(${newsofile} ${plugin} ${pluginHeader} ${${pluginTemplate}} ${${plugin_add_srcsname}})
  SET_TARGET_PROPERTIES(${newsofile} PROPERTIES
                         COMPILE_FLAGS "${${PROJECT_NAME}_CXXFLAGS}"
                         PREFIX ""
  			 LIBRARY_OUTPUT_DIRECTORY "${${PROJECT_NAME}_BINARY_DIR}/lib/plugin")

  # Make sure that this plugin depends upon the other specified dependencies.
  SET(newsofile_plugins_dependencies  ${${newsofile}_plugins_dependencies})
  SET(newsofile_plugins_compile_flags ${${newsofile}_plugins_compile_flags})
  SET(newsofile_plugins_link_flags    ${${newsofile}_plugins_link_flags})
  
  SET(localtarget_dependencies ${LIBRARY_NAME}    ${newsofile_plugins_dependencies})
  SET(local_plugins_compile    ${${PROJECT_NAME}_CXX_FLAGS}      ${newsofile_plugins_compile_flags})
  SET(local_plugins_link_flags ${${PROJECT_NAME}_PLUGIN_LDFLAGS} ${newsofile_plugins_link_flags} )
	  
  # Make sure that this plugin is depend upon the global dependancies.
  foreach (dependencyOrder ${localtarget_dependencies})
		ADD_DEPENDENCIES(${newsofile} ${dependencyOrder})
  endforeach (dependencyOrder)

  link_directories(${LIBRARY_OUTPUT_PATH})
  target_link_libraries(${newsofile} ${LIBRARY_NAME})

  foreach (extraDependency ${newsofile_plugins_dependencies})
    # Add the flags of the sub plugins.
    LIST(APPEND local_plugins_compile ${${extraDependency}_plugins_compile_flags})
    target_link_libraries(${newsofile} ${extraDependency})
  endforeach (extraDependency)


  
  LIST2STRING(_cf "${local_plugins_compile}")
  LIST2STRING(_lf "${local_plugins_link_flags}")
  SET_TARGET_PROPERTIES(${newsofile} PROPERTIES 
   	 		 COMPILE_FLAGS "${_cf} ${${PROJECT_NAME}_CXXFLAGS}"
         LINK_FLAGS "${_lf}")
  INSTALL(TARGETS ${newsofile} 
	   DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plugin
  	   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
  ADD_DEPENDENCIES(${newsofile} header_${newsofile})
ENDFOREACH(plugin)
