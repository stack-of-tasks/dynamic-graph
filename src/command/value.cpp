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

    AnyType::AnyType(const Value& value) : value_(value)
    {
    }
    AnyType::operator int () const
    {
      return value_.intValue();
    }
    AnyType::operator double () const
    {
      return value_.doubleValue();
    }
    AnyType::operator std::string () const
    {
      return value_.stringValue();
    }

    Value::~Value()
    {
      switch(type_) {
      case INT:
	delete (int*)value_;
	break;
      case DOUBLE:
	delete (double*)value_;
	break;
      case STRING:
	delete (std::string*)value_;
	break;
      }
    }

    Value::Value(const int& value)
    {
      std::cout << "Constructor of int value" << std::endl;
      value_ = new int(value);
      type_ = INT;
    }
    Value::Value(const double& value)
    {
      std::cout << "Constructor of double value" << std::endl;
      value_ = new double(value);
      type_ = DOUBLE;
    }
    Value::Value(const std::string& value)
    {
      std::cout << "Constructor of string value" << std::endl;
      value_ = new std::string(value);
      type_ = STRING;
    }


    Value::Value(const Value& value) : type_(value.type_)
    {
      switch(value.type_) {
      case INT:
	std::cout << "Value copy constructor: int" << std::endl;
	value_ = new int(value.intValue());
	break;
      case DOUBLE:
	std::cout << "Value copy constructor: double" << std::endl;
	value_ = new double(value.doubleValue());
	break;
      case STRING:
	std::cout << "Value copy constructor: string" << std::endl;
	value_ = new std::string(value.stringValue());
	break;
      }
    }

    Value::Value() : type_(NONE), value_(NULL)
    {
      std::cout << "Value empty constructor" << std::endl;
    }

    const AnyType Value::value() const
    {
      return AnyType(*this);
    }

    Value::Type Value::type() const
    {
      return type_;
    }

    const double Value::doubleValue () const
    {
      double result;
      if (type_ == DOUBLE)
	result = *((double*)value_);
      std::cout << "Value::doubleValue = " << result << std::endl;
      return result;
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a double");
    }

    const int Value::intValue () const
    {
      if (type_ == INT)
	return *((int*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an int");
    }

    const std::string Value::stringValue () const
    {
      if (type_ == STRING)
	return *((std::string*)value_);
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

    std::ostream& operator<<(std::ostream& os, const Value& value)
    {
      os << "Type=" << Value::typeName(value.type_)
	 << ", value=";
      switch (value.type_) {
      case Value::INT:
	os << value.intValue();
	break;
      case Value::DOUBLE:
	os << value.doubleValue();
	break;
      case Value::STRING:
	os << value.stringValue();
	break;
      }
      return os;
    }

  } // namespace command
} //namespace dynamicgraph
