// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_EXCEPTION_SIGNAL_H
#define DYNAMIC_GRAPH_EXCEPTION_SIGNAL_H

#include <dynamic-graph/fwd.hh>
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-abstract.h>

namespace dynamicgraph {
/// \ingroup error
///
/// \brief Exceptions raised when an error related to signals
/// happen.
class DYNAMIC_GRAPH_DLLAPI ExceptionSignal : public ExceptionAbstract {
 public:
  enum ErrorCodeEnum {
    GENERIC = ExceptionAbstract::SIGNAL

    ,
    READWRITE_LOCK,
    COPY_NOT_INITIALIZED,
    NOT_INITIALIZED,
    PLUG_IMPOSSIBLE,
    SET_IMPOSSIBLE,
    BAD_CAST
  };

  static const std::string EXCEPTION_NAME;

  explicit ExceptionSignal(const ExceptionSignal::ErrorCodeEnum& errcode, const std::string& msg = "");
  ExceptionSignal(const ExceptionSignal::ErrorCodeEnum& errcode, const std::string& msg, const char* format, ...);
  virtual ~ExceptionSignal() throw() {}

  virtual const std::string& getExceptionName() const { return EXCEPTION_NAME; }
};

}  // end of namespace dynamicgraph

#endif  //! DYNAMIC_GRAPH_EXCEPTION_SIGNAL_H
