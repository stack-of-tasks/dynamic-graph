# ---------------------------------------------------------------
# --- SOURCE FILE NAMES -----------------------------------------
# ---------------------------------------------------------------
SET(SOT_MODULE_NAME exception)

# --- Add your Source Files here.
SET(libdg_${SOT_MODULE_NAME}_src 
      exception/exception-abstract.cpp 
      exception/exception-factory.cpp  
      exception/exception-signal.cpp 
)

SET (libdg_${SOT_MODULE_NAME}_templates "")
SET(libdg_${SOT_MODULE_NAME}_headers "")

# ---------------------------------------------------------------
# --- GENERIC RULES ---------------------------------------------
# ---------------------------------------------------------------
SOT_SRC_SUBDIR_PROCESS()
