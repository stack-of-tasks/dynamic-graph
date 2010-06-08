# ---------------------------------------------------------------
# --- SOURCE FILE NAMES -----------------------------------------
# ---------------------------------------------------------------
SET(SOT_MODULE_NAME debug)

# --- Add your Source Files here.
SET(libdg_${SOT_MODULE_NAME}_src
      debug/debug.cpp
)
SET(libdg_${SOT_MODULE_NAME}_plugins_src 
      debug/contiifstream.cpp
)

# ---------------------------------------------------------------
# --- GENERIC RULES ---------------------------------------------
# ---------------------------------------------------------------
SOT_SRC_SUBDIR_PROCESS()
