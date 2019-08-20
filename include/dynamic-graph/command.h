//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#ifndef DYNAMIC_GRAPH_COMMAND_H
#define DYNAMIC_GRAPH_COMMAND_H

#include "dynamic-graph/dynamic-graph-api.h"
#include "dynamic-graph/value.h"
#include <vector>

namespace dynamicgraph {
class Entity;
namespace command {
/// Abstract class for entity commands
///
/// This class provide a mean to control entities from external
/// python script.
///
/// A command
/// \li is owned by an entity,
/// \li takes parameters of type Value,
/// \li return an instance of Value when calling Command::execute()
///
/// At construction, the prototype of the command is defined by providing
/// a vector of Value::Type.
///
/// Parameters are set by calling Command::setParameterValues with a
/// vector of Values the types of which should fit the vector specified
/// at construction.
class DYNAMIC_GRAPH_DLLAPI Command {
public:
  virtual ~Command();
  /// Store the owner entity and a vector of value types
  /// \param entity reference to Entity owning this command.
  /// \param valueTypes vector specifying the number and types of parameters
  /// \param docstring documentation of the command
  Command(Entity &entity, const std::vector<Value::Type> &valueTypes,
          const std::string &docstring);
  /// Return the value type of all parameters
  const std::vector<Value::Type> &valueTypes() const;
  /// Set parameter values
  void setParameterValues(const std::vector<Value> &values);
  /// Get parameter values
  const std::vector<Value> &getParameterValues() const;
  /// Execute the command after checking parameters
  Value execute();
  /// Get a reference to the Entity owning this command
  Entity &owner();
  /// Get documentation string
  std::string getDocstring() const;

protected:
  /// Specific action performed by the command
  virtual Value doExecute() = 0;

private:
  Entity &owner_;
  std::vector<Value::Type> valueTypeVector_;
  std::vector<Value> valueVector_;
  std::string docstring_;

public:
  static const std::vector<Value::Type> EMPTY_ARG;
};
} // namespace command
} // namespace dynamicgraph

#endif // DYNAMIC_GRAPH_COMMAND_H
