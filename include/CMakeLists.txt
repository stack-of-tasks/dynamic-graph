# Generate header with default script directory.
SET(DG_IMPORT_DEFAULT_PATHS \"${CMAKE_INSTALL_PREFIX}/script\")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/import-default-paths.h.cmake
		       ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/import-default-paths.h)

SET(${PROJECT_NAME}_HEADERS
contiifstream.h
debug.h

dynamic-graph-api.h

entity.h
factory.h
interpreter.h
interpreter-helper.h
plugin-loader.h
pool.h
import.h
import-default-paths.h

exception-abstract.h
exception-factory.h
exception-signal.h
exception-traces.h

functions.h
shell-procedure.h

signal.h
signal-array.h
signal-base.h
signal-ptr.h
signal-time-dependent.h
signal-ptr.t.cpp
signal.t.cpp
time-dependency.h
time-dependency.t.cpp
signal-caster.h
all-signals.h

tracer.h
tracer-real-time.h
)

# Recreate correct path for the headers
#--------------------------------------

SET(fullpath_${PROJECT_NAME}_HEADERS)
FOREACH(lHeader ${${PROJECT_NAME}_HEADERS})
 SET(fullpath_${PROJECT_NAME}_HEADERS
   ${fullpath_${PROJECT_NAME}_HEADERS}
   ./${PROJECT_NAME}/${lHeader}
 )

ENDFOREACH(lHeader)

#----------------------------------------------------
# Install procedure for the header files
#----------------------------------------------------
INSTALL(FILES ${fullpath_${PROJECT_NAME}_HEADERS}
       DESTINATION include/${PROJECT_NAME}
       PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE
)