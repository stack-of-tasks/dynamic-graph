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

#include "dynamic-graph/value.h"
#include "dynamic-graph/exception-abstract.h"

namespace dynamicgraph {
  namespace command {

    Value::Value(const Value& value) : type_(value.type_),
				       value_(value.value_)
    {
    }

    Value::Value() : type_(NONE), value_(NULL)
    {
    }

    Value::Type Value::type() const
    {
      return type_;
    }

    const double& Value::doubleValue () const 
    {
      if (type_ == DOUBLE)
	return *(static_cast<const double*>(value_));
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a double");
    }

    const int& Value::intValue () const
    {
      if (type_ == INT)
	return *(static_cast<const int*>(value_));
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an int");
    }
    
    const std::string& Value::stringValue () const
    {
      if (type_ == STRING)
	return *(static_cast<const std::string*>(value_));
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an string");
    }
    
    std::string Value::typeName(Type type)
    {
      switch (type) {
      case INT:
	return std::string("int");
      case DOUBLE:
	return std::string("double");
      case STRING:
	return std::string("string");
      }
      return std::string("unknown");
    }
  } // namespace command
} //namespace dynamicgraph
