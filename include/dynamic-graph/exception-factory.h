// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_EXCEPTION_FACTORY_H
#define DYNAMIC_GRAPH_EXCEPTION_FACTORY_H
#include <string>

#include <dynamic-graph/fwd.hh>
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-abstract.h>

namespace dynamicgraph {
/// \ingroup error
///
/// \brief Generic error class.
class DYNAMIC_GRAPH_DLLAPI ExceptionFactory : public ExceptionAbstract {
 public:
  enum ErrorCodeEnum {
    GENERIC = ExceptionAbstract::FACTORY,
    UNREFERED_OBJECT,
    UNREFERED_SIGNAL,
    UNREFERED_FUNCTION,
    DYNAMIC_LOADING,
    SIGNAL_CONFLICT,
    FUNCTION_CONFLICT,
    OBJECT_CONFLICT,
    SYNTAX_ERROR,
    READ_FILE
  };

  static const std::string EXCEPTION_NAME;

  explicit ExceptionFactory(const ExceptionFactory::ErrorCodeEnum& errcode, const std::string& msg = "");

  ExceptionFactory(const ExceptionFactory::ErrorCodeEnum& errcode, const std::string& msg, const char* format, ...);

  virtual ~ExceptionFactory() throw() {}

  virtual const std::string& getExceptionName() const { return ExceptionFactory::EXCEPTION_NAME; }
};
}  // end of namespace dynamicgraph

#endif  //! DYNAMIC_GRAPH_EXCEPTION_FACTORY_H
