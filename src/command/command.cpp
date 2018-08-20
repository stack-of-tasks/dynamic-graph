//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//
// This file is part of dynamic-graph.
// dynamic-graph is free software:
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
