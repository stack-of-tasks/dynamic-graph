# ---------------------------------------------------------------
# --- SOURCE FILE NAMES -----------------------------------------
# ---------------------------------------------------------------
SET(SOT_MODULE_NAME signal)

# --- Add your Source Files here.
SET (libdg_${SOT_MODULE_NAME}_src
      signal/signal-array.cpp
)

SET (libdg_${SOT_MODULE_NAME}_templates 
      signal/signal.t.cpp 
      signal/time-dependency.t.cpp 
      signal/signal-ptr.t.cpp
)

SET(libdg_${SOT_MODULE_NAME}_headers 
     signal/signal-base.h
     signal/signal-time-dependant.h
)

# Add specific compilation rules.

# ---------------------------------------------------------------
# --- GENERIC RULES ---------------------------------------------
# ---------------------------------------------------------------
SOT_SRC_SUBDIR_PROCESS()
