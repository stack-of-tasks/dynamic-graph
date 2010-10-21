//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
// dynamic-graph is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.  You should
// have received a copy of the GNU Lesser General Public License along
// with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#include <sstream>
#include "dynamic-graph/command.h"
#include "dynamic-graph/exception-abstract.h"

namespace dynamicgraph {
  namespace command {

    Command::~Command() {}
    Command::Command(Entity& entity,
		     const std::vector<Value::Type>& valueTypes) : 
      owner_(entity), valueTypeVector_(valueTypes)
    {
    }

    const std::vector<Value::Type>& Command::valueTypes() const
    {
      return valueTypeVector_;
    }

    void Command::setParameterValues(const std::vector<Value>& values)
    {
      unsigned int size = values.size();
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
	  ss << "argument " << iParam << "is of wrong type: "
	     << Value::typeName(paramTypes[iParam]) << " expected, got "
	     << Value::typeName(values[iParam].type());
	  throw ExceptionAbstract(ExceptionAbstract::TOOLS, ss.str());
	}
      }
      // Copy vector of values in private part
      valueVector_ = values;
    }

    const std::vector<Value> Command::getParameterValues()
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
  } // namespace command
} //namespace dynamicgraph
