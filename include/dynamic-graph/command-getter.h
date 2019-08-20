//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#ifndef DYNAMIC_GRAPH_COMMAND_GETTER_H
#define DYNAMIC_GRAPH_COMMAND_GETTER_H

#include "dynamic-graph/command.h"

namespace dynamicgraph {
namespace command {

///
/// Command that calls a parameter getter function
///
/// This class is templated by a type E deriving from entity and
/// a type T of data.
///
/// Let us assume that class E has a private member of type T and a
/// public getter function for this member:
/// \code
/// class E : public Entity
/// {
/// public:
///   E (const std::string& inName) : Entity(inName) {}
///   T getParameter() const {return parameter_;}
/// private:
///   T parameter_;
/// };
/// \endcode
/// Then the command defined by:
/// \code
/// E entity("MyEntity");
/// Getter<E,T> command(entity, &E::getParameter)
/// \endcode
/// returns the value of <c>entity.parameter_</c> upon invocation.
///
/// \note
/// \li T should be a type supported by class Value,
/// \li prototype of E::getParameter should be exactly as specified in this
/// example.
template <class E, typename T> class Getter : public Command {
public:
  /// Pointer to method that sets parameter of type T
  typedef T (E::*GetterMethod)() const;
  /// Constructor
  Getter(E &entity, GetterMethod getterMethod, const std::string &docString);

protected:
  virtual Value doExecute();

private:
  GetterMethod getterMethod_;
};
} // namespace command
} // namespace dynamicgraph

#include "dynamic-graph/command-getter.t.cpp"
#endif // DYNAMIC_GRAPH_COMMAND_GETTER_H
