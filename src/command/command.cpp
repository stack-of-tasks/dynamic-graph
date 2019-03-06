//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#include <sstream>
#include "dynamic-graph/command.h"
#include "dynamic-graph/exception-abstract.h"

namespace dynamicgraph {
  namespace command {

    const std::vector<Value::Type> Command::EMPTY_ARG = std::vector<Value::Type>();


    Command::~Command() {}
    Command::Command(Entity& entity,
		     const std::vector<Value::Type>& valueTypes,
		     const std::string& docstring) :
      owner_(entity), valueTypeVector_(valueTypes), docstring_(docstring)
    {
    }

    const std::vector<Value::Type>& Command::valueTypes() const
    {
      return valueTypeVector_;
    }

    void Command::setParameterValues(const std::vector<Value>& values)
    {
      const std::vector<Value::Type>& paramTypes = valueTypes();
      // Check that number of parameters is correct
      if (values.size() != paramTypes.size()) {
	throw ExceptionAbstract(ExceptionAbstract::ABSTRACT,
				"wrong number of parameters");
      }
      // Check that each parameter is of correct type
      for (unsigned int iParam=0; iParam < values.size(); iParam++) {
	if (values[iParam].type() != paramTypes[iParam]) {
	  std::stringstream ss;
	  ss << "argument " << iParam << " is of wrong type: "
	     << Value::typeName(paramTypes[iParam]) << " expected, got "
	     << Value::typeName(values[iParam].type());
	  throw ExceptionAbstract(ExceptionAbstract::TOOLS, ss.str());
	}
      }
      // Copy vector of values in private part
      valueVector_ = values;
    }

    const std::vector<Value>& Command::getParameterValues() const
    {
      return valueVector_;
    }

    Value Command::execute()
    {
      return doExecute();
    }

    Entity& Command::owner()
    {
      return owner_;
    }
    std::string Command::getDocstring() const
    {
      return docstring_;
    }
  } // namespace command
} //namespace dynamicgraph
