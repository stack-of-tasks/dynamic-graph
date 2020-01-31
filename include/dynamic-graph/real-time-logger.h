// -*- mode: c++ -*-
// Copyright 2018, Joseph Mirabel LAAS-CNRS
//

#ifndef DYNAMIC_GRAPH_LOGGER_REAL_TIME_H
#define DYNAMIC_GRAPH_LOGGER_REAL_TIME_H

#ifdef ENABLE_RT_LOG
#define dgADD_OSTREAM_TO_RTLOG(ostr)                                           \
  ::dynamicgraph::RealTimeLogger::instance().addOutputStream(                  \
      ::dynamicgraph::LoggerStreamPtr_t(                                       \
          new ::dynamicgraph::LoggerIOStream(ostr)))

#define dgRTLOG() ::dynamicgraph::RealTimeLogger::instance().front()
#else // ENABLE_RT_LOG
#define dgADD_OSTREAM_TO_RTLOG(ostr) struct __end_with_semicolon
#define dgRTLOG()                                                              \
  if (1)                                                                       \
    ;                                                                          \
  else                                                                         \
    __null_stream()
#endif

#include <dynamic-graph/real-time-logger-def.h>

#endif //! DYNAMIC_GRAPH_LOGGER_REAL_TIME_H
