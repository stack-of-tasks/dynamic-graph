//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#ifndef DYNAMIC_GRAPH_COMMAND_SETTER_H
#define DYNAMIC_GRAPH_COMMAND_SETTER_H

#include "dynamic-graph/command.h"

namespace dynamicgraph {
namespace command {

///
/// Command that calls a parameter setter function
///
/// This class is templated by a type E deriving from entity and
/// a type T of data.
///
/// Let us assume that class E has a private member of type T and a
/// public setter function for this member:
/// \code
/// class E : public Entity
/// {
/// public:
///   E (const std::string& inName) : Entity(inName) {}
///   void setParameter(const T& parameter) {parameter_ = parameter;}
/// private:
///   T parameter_;
/// };
/// \endcode
/// Then the command defined by:
/// \code
/// E entity("MyEntity");
/// Setter<E,T> command(entity, &E::getParameter)
/// \endcode
/// sets the value of <c>entity.parameter_</c> upon invocation.
///
/// \note
/// \li T should be a type supported by class Value,
/// \li prototype of E::setParameter should be exactly as specified in this
/// example.
template <class E, typename T> class Setter : public Command {
public:
  /// Pointer to method that sets parameter of type T
  typedef void (E::*SetterMethod)(const T &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

protected:
  virtual Value doExecute();

private:
  SetterMethod setterMethod_;
};
} // namespace command
} // namespace dynamicgraph

#include "dynamic-graph/command-setter.t.cpp"
#endif // DYNAMIC_GRAPH_COMMAND_SETTER_H
