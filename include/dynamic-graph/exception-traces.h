// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_EXCEPTION_TRACES_H
#define DYNAMIC_GRAPH_EXCEPTION_TRACES_H
#include <string>

#include <dynamic-graph/fwd.hh>
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-abstract.h>

namespace dynamicgraph {
/// \ingroup error
///
/// \brief Exceptions raised when an error related to traces happen.
class DYNAMIC_GRAPH_DLLAPI ExceptionTraces : public ExceptionAbstract {
 public:
  enum ErrorCodeEnum { GENERIC = ExceptionAbstract::TRACES, NOT_OPEN };

  static const std::string EXCEPTION_NAME;

  explicit ExceptionTraces(const ExceptionTraces::ErrorCodeEnum& errcode, const std::string& msg = "");
  ExceptionTraces(const ExceptionTraces::ErrorCodeEnum& errcode, const std::string& msg, const char* format, ...);
  virtual ~ExceptionTraces() throw() {}

  virtual const std::string& getExceptionName() const { return EXCEPTION_NAME; }
};
}  // end of namespace dynamicgraph.

#endif  //! DYNAMIC_GRAPH_EXCEPTION_TRACES_H
