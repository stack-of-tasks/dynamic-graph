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

    EitherType::EitherType(const Value& value) : value_(new Value(value))
    {
    }

    EitherType::~EitherType()
    {
      delete value_;
    }

    EitherType::operator bool () const
    {
      return value_->boolValue();
    }
    EitherType::operator unsigned () const
    {
      return value_->unsignedValue();
    }
    EitherType::operator int () const
    {
      return value_->intValue();
    }
    EitherType::operator float () const
    {
      return value_->floatValue();
    }
    EitherType::operator double () const
    {
      return value_->doubleValue();
    }
    EitherType::operator std::string () const
    {
      return value_->stringValue();
    }

    Value::~Value()
    {
      switch(type_) {
      case BOOL:
	delete (bool*)value_;
	break;
      case UNSIGNED:
	delete (unsigned*)value_;
	break;
      case INT:
	delete (int*)value_;
	break;
      case FLOAT:
	delete (float*)value_;
	break;
      case DOUBLE:
	delete (double*)value_;
	break;
      case STRING:
	delete (std::string*)value_;
	break;
      }
    }

    Value::Value(const bool& value)
    {
      value_ = new bool(value);
      type_ = BOOL;
    }
    Value::Value(const unsigned& value)
    {
      value_ = new unsigned(value);
      type_ = UNSIGNED;
    }
    Value::Value(const int& value)
    {
      value_ = new int(value);
      type_ = INT;
    }
    Value::Value(const float& value)
    {
      value_ = new float(value);
      type_ = FLOAT;
    }
    Value::Value(const double& value)
    {
      value_ = new double(value);
      type_ = DOUBLE;
    }
    Value::Value(const std::string& value)
    {
      value_ = new std::string(value);
      type_ = STRING;
    }


    Value::Value(const Value& value) : type_(value.type_)
    {
      switch(value.type_) {
      case BOOL:
	value_ = new bool(value.intValue());
	break;
      case UNSIGNED:
	value_ = new unsigned(value.intValue());
	break;
      case INT:
	value_ = new int(value.intValue());
	break;
      case FLOAT:
	value_ = new float(value.intValue());
	break;
      case DOUBLE:
	value_ = new double(value.doubleValue());
	break;
      case STRING:
	value_ = new std::string(value.stringValue());
	break;
      default:
	type_ = NONE;
	value_ = NULL;
      }
    }

    Value::Value() : type_(NONE), value_(NULL)
    {
    }

    const EitherType Value::value() const
    {
      return EitherType(*this);
    }

    Value::Type Value::type() const
    {
      return type_;
    }

    const bool Value::boolValue () const
    {
      if (type_ == BOOL)
	return *((bool*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an bool");
    }

    const unsigned Value::unsignedValue () const
    {
      if (type_ == UNSIGNED)
	return *((unsigned*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an unsigned int");
    }

    const int Value::intValue () const
    {
      if (type_ == INT)
	return *((int*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an int int");
    }

    const float Value::floatValue () const
    {
      float result;
      if (type_ == FLOAT)
	result = *((float*)value_);
      return result;
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a float");
    }

    const double Value::doubleValue () const
    {
      double result;
      if (type_ == DOUBLE)
	result = *((double*)value_);
      return result;
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a double");
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
      case BOOL:
	return std::string("bool");
      case UNSIGNED:
	return std::string("unsigned int");
      case INT:
	return std::string("int");
      case FLOAT:
	return std::string("float");
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
      case Value::BOOL:
	os << value.boolValue();
	break;
      case Value::UNSIGNED:
	os << value.unsignedValue();
	break;
      case Value::INT:
	os << value.intValue();
	break;
      case Value::DOUBLE:
	os << value.doubleValue();
	break;
      case Value::FLOAT:
	os << value.floatValue();
	break;
      case Value::STRING:
	os << value.stringValue();
	break;
      }
      return os;
    }

  } // namespace command
} //namespace dynamicgraph
