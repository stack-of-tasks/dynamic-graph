// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_DEBUG_HH
#define DYNAMIC_GRAPH_DEBUG_HH
#include <dynamic-graph/dynamic-graph-api.h>

#include <cstdarg>
#include <cstdio>
#include <dynamic-graph/fwd.hh>
#include <fstream>
#include <sstream>

#ifndef VP_DEBUG_MODE
#define VP_DEBUG_MODE 0
#endif  //! VP_DEBUG_MODE

#ifndef VP_TEMPLATE_DEBUG_MODE
#define VP_TEMPLATE_DEBUG_MODE 0
#endif  //! VP_TEMPLATE_DEBUG_MODE

#define DG_COMMON_TRACES                                        \
  do {                                                          \
    va_list arg;                                                \
    va_start(arg, format);                                      \
    vsnprintf(charbuffer, SIZE, format, arg);                   \
    va_end(arg);                                                \
    outputbuffer << tmpbuffer.str() << charbuffer << std::endl; \
  } while (0)

namespace dynamicgraph {
/// \ingroup debug
///
/// \brief Logging class.
///
/// This class should never be used directly, please use the
/// debugging macro instead.
class DYNAMIC_GRAPH_DLLAPI DebugTrace {
 public:
  static const int SIZE = 512;

  std::stringstream tmpbuffer;
  std::ostream &outputbuffer;
  char charbuffer[SIZE + 1];
  int traceLevel;
  int traceLevelTemplate;

  DebugTrace(std::ostream &os) : outputbuffer(os) {}

  inline void trace(const int level, const char *format, ...) {
    if (level <= traceLevel) DG_COMMON_TRACES;
    tmpbuffer.str("");
  }

  inline void trace(const char *format, ...) {
    DG_COMMON_TRACES;
    tmpbuffer.str("");
  }

  inline void trace(const int level = -1) {
    if (level <= traceLevel) {
      outputbuffer << tmpbuffer.str();
      tmpbuffer.str("");
    }
  }

  inline void traceTemplate(const int level, const char *format, ...) {
    if (level <= traceLevelTemplate) DG_COMMON_TRACES;
    tmpbuffer.str("");
  }

  inline void traceTemplate(const char *format, ...) {
    DG_COMMON_TRACES;
    tmpbuffer.str("");
  }

  inline DebugTrace &pre(const std::ostream &) { return *this; }

  inline DebugTrace &pre(const std::ostream &, int level) {
    traceLevel = level;
    return *this;
  }

  static const char *DEBUG_FILENAME_DEFAULT;
  static void openFile(const char *filename = DEBUG_FILENAME_DEFAULT);
  static void closeFile(const char *filename = DEBUG_FILENAME_DEFAULT);
};

DYNAMIC_GRAPH_DLLAPI extern DebugTrace dgDEBUGFLOW;
DYNAMIC_GRAPH_DLLAPI extern DebugTrace dgERRORFLOW;
}  // end of namespace dynamicgraph

#ifdef VP_DEBUG

#define dgPREDEBUG __FILE__ << ": " << __FUNCTION__ << "(#" << __LINE__ << ") :"
#define dgPREERROR \
  "\t!! " << __FILE__ << ": " << __FUNCTION__ << "(#" << __LINE__ << ") :"

#define dgDEBUG(level)                                               \
  if ((level > VP_DEBUG_MODE) || (!dgDEBUGFLOW.outputbuffer.good())) \
    ;                                                                \
  else                                                               \
    dgDEBUGFLOW.outputbuffer << dgPREDEBUG

#define dgDEBUGMUTE(level)                                           \
  if ((level > VP_DEBUG_MODE) || (!dgDEBUGFLOW.outputbuffer.good())) \
    ;                                                                \
  else                                                               \
    dgDEBUGFLOW.outputbuffer

#define dgERROR                         \
  if (!dgDEBUGFLOW.outputbuffer.good()) \
    ;                                   \
  else                                  \
    dgERRORFLOW.outputbuffer << dgPREERROR

#define dgDEBUGF                        \
  if (!dgDEBUGFLOW.outputbuffer.good()) \
    ;                                   \
  else                                  \
    dgDEBUGFLOW.pre(dgDEBUGFLOW.tmpbuffer << dgPREDEBUG, VP_DEBUG_MODE).trace

#define dgERRORF                        \
  if (!dgDEBUGFLOW.outputbuffer.good()) \
    ;                                   \
  else                                  \
    dgERRORFLOW.pre(dgERRORFLOW.tmpbuffer << dgPREERROR).trace

// TEMPLATE
#define dgTDEBUG(level)                                                       \
  if ((level > VP_TEMPLATE_DEBUG_MODE) || (!dgDEBUGFLOW.outputbuffer.good())) \
    ;                                                                         \
  else                                                                        \
    dgDEBUGFLOW.outputbuffer << dgPREDEBUG

#define dgTDEBUGF                                                         \
  if (!dgDEBUGFLOW.outputbuffer.good())                                   \
    ;                                                                     \
  else                                                                    \
    dgDEBUGFLOW                                                           \
        .pre(dgDEBUGFLOW.tmpbuffer << dgPREDEBUG, VP_TEMPLATE_DEBUG_MODE) \
        .trace

inline bool dgDEBUG_ENABLE(const int &level) { return level <= VP_DEBUG_MODE; }

inline bool dgTDEBUG_ENABLE(const int &level) {
  return level <= VP_TEMPLATE_DEBUG_MODE;
}

#else  // VP_DEBUG

#define dgPREERROR \
  "\t!! " << __FILE__ << ": " << __FUNCTION__ << "(#" << __LINE__ << ") :"

#define dgDEBUG(level) \
  if (1)               \
    ;                  \
  else                 \
    ::dynamicgraph::__null_stream()

#define dgDEBUGMUTE \
  (level) if (1);   \
  else ::dynamicgraph::__null_stream()

#define dgERROR dgERRORFLOW.outputbuffer << dgPREERROR

inline void dgDEBUGF(const int, const char *, ...) { return; }

inline void dgDEBUGF(const char *, ...) { return; }

inline void dgERRORF(const int, const char *, ...) { return; }

inline void dgERRORF(const char *, ...) { return; }

namespace dynamicgraph {
inline std::ostream &__null_stream() {
  // This function should never be called. With -O3,
  // it should not appear in the generated binary.
  static std::ostream os(NULL);
  return os;
}
}  // namespace dynamicgraph

// TEMPLATE
#define dgTDEBUG(level) \
  if (1)                \
    ;                   \
  else                  \
    ::dynamicgraph::__null_stream()

inline void dgTDEBUGF(const int, const char *, ...) { return; }

inline void dgTDEBUGF(const char *, ...) { return; }

#define dgDEBUG_ENABLE(level) false
#define dgTDEBUG_ENABLE(level) false

#endif  //! VP_DEBUG

#define dgDEBUGIN(level) dgDEBUG(level) << "# In {" << std::endl

#define dgDEBUGOUT(level) dgDEBUG(level) << "# Out }" << std::endl

#define dgDEBUGINOUT(level) dgDEBUG(level) << "# In/Out { }" << std::endl

#define dgTDEBUGIN(level) dgTDEBUG(level) << "# In {" << std::endl

#define dgTDEBUGOUT(level) dgTDEBUG(level) << "# Out }" << std::endl

#define dgTDEBUGINOUT(level) dgTDEBUG(level) << "# In/Out { }" << std::endl

#endif  //! DYNAMIC_GRAPH_DEBUG_HH
