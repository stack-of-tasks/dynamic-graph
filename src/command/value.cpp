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

#include <boost/numeric/ublas/io.hpp>
#include "dynamic-graph/value.h"
#include "dynamic-graph/exception-abstract.h"

namespace dynamicgraph {
  namespace command {

    static void* copyValue(const Value& value);

    EitherType::EitherType(const Value& value) : value_(new Value(value))
    {
    }

    EitherType::~EitherType()
    {
      delete value_;
      value_ = NULL;
    }

    EitherType::operator bool() const
    {
      return value_->boolValue();
    }
    EitherType::operator unsigned() const
    {
      return value_->unsignedValue();
    }
    EitherType::operator int() const
    {
      return value_->intValue();
    }
    EitherType::operator float() const
    {
      return value_->floatValue();
    }
    EitherType::operator double() const
    {
      return value_->doubleValue();
    }
    EitherType::operator std::string() const
    {
      return value_->stringValue();
    }
    EitherType::operator Vector() const
    {
      return value_->vectorValue();
    }
    EitherType::operator Matrix() const
    {
      return value_->matrixValue();
    }

    Value::~Value()
    {
      std::cout << "Deleting Value at " << this ;
      switch(type_) {
      case BOOL:
	std::cout << " type bool at " << value_;
	delete(bool*)value_;
	break;
      case UNSIGNED:
	delete(unsigned*)value_;
	std::cout << " type unsigned at " << value_;
	break;
      case INT:
	delete(int*)value_;
	std::cout << " type int at " << value_;
	break;
      case FLOAT:
	delete(float*)value_;
	std::cout << " type float at " << value_;
	break;
      case DOUBLE:
	delete(double*)value_;
	std::cout << " type double at " << value_;
	break;
      case STRING:
	delete(std::string*)value_;
	std::cout << " type std::string at " << value_;
	break;
      case VECTOR:
	delete(Vector*)value_;
	std::cout << " type vector at " << value_;
	break;
      case MATRIX:
	delete(Matrix*)value_;
	std::cout << " type matrix at " << value_;
	break;
      default:;
      }
      std::cout << std::endl;
    }

    Value::Value(const bool& value) : type_(BOOL), value_(new bool(value))
    {
      std::cout << "New Value bool at " << this 
		<< " pointing at " << value_ << std::endl;
    }
    Value::Value(const unsigned& value) : type_(UNSIGNED),
					  value_(new unsigned(value))
    {
      std::cout << "New Value unsigned at " 
		<< " pointing at " << value_ << this << std::endl;
    }
    Value::Value(const int& value) : type_(INT),
				     value_(new int(value))
    {
      std::cout << "New Value int at " << this 
		<< " pointing at " << value_ << std::endl;
    }
    Value::Value(const float& value) : type_(FLOAT),
				       value_(new float(value))
    {
      std::cout << "New Value float at " << this 
		<< " pointing at " << value_ << std::endl;
    }
    Value::Value(const double& value) : type_(DOUBLE),
					value_(new double(value))
    {
      std::cout << "New Value double at " << this 
		<< " pointing at " << value_ << std::endl;
    }
    Value::Value(const std::string& value) : type_(STRING),
					     value_(new std::string(value))
    {
      std::cout << "New Value string at " << this 
		<< " pointing at " << value_ << std::endl;
    }
    Value::Value(const Vector& value) : type_(VECTOR),
					value_(new Vector(value))
    {
      std::cout << "New Value Vector at " << this << " pointing at "
		<< value_ << std::endl;
    }
    Value::Value(const Matrix& value) : type_(MATRIX),
					value_(new Matrix(value))
    {
      std::cout << "New Value Matrix at " << this 
		<< " pointing at " << value_ << std::endl;
    }


    Value::Value(const Value& value) : type_(value.type_),
				       value_(copyValue(value))
    {
    }

    void* copyValue(const Value& value)
    {
      void* copy;
      std::cout << "copy: ";
      switch(value.type()) {
      case Value::NONE:
	copy = NULL;
	std::cout << "New Value None at " << copy 
		  << " pointing at " << copy << std::endl;
      case Value::BOOL:
	copy = new bool(value.boolValue());
	std::cout << "New Value bool at " << copy 
		  << " pointing at " << copy << std::endl;
	break;
      case Value::UNSIGNED:
	copy = new unsigned(value.unsignedValue());
	std::cout << "New Value unsigned at " << copy 
		  << " pointing at " << copy << std::endl;
	break;
      case Value::INT:
	copy = new int(value.intValue());
	std::cout << "New Value int at " << copy 
		  <<" pointing at " << copy << std::endl;
	break;
      case Value::FLOAT:
	copy = new float(value.floatValue());
	std::cout << "New Value float at " << copy 
		  << " pointing at " << copy << std::endl;
	break;
      case Value::DOUBLE:
	copy = new double(value.doubleValue());
	std::cout << "New Value double at " << copy 
		  << " pointing at " << copy << std::endl;
	break;
      case Value::STRING:
	copy = new std::string(value.stringValue());
	std::cout << "New Value string at " << copy 
		  << " pointing at " << copy << std::endl;
	break;
      case Value::VECTOR:
	copy = new Vector(value.vectorValue());
	std::cout << "New Value vector at " << copy 
		  << " pointing at " << copy << std::endl;
	break;
      case Value::MATRIX:
	copy = new Matrix(value.matrixValue());
	std::cout << "New Value matrix at " << copy 
		  << " pointing at " << copy << std::endl;
	break;
      default:
	abort();
      }
      return copy;
    }

    Value::Value() : type_(NONE), value_(NULL)
    {
      std::cout << "New Value none at " << this 
		<< " pointing at " << value_ << std::endl;
    }

    Value Value::operator=(const Value& value)
    {
      return Value (value);
    }

    const EitherType Value::value() const
    {
      return EitherType(*this);
    }

    Value::Type Value::type() const
    {
      return type_;
    }

    bool Value::boolValue() const
    {
      if(type_ == BOOL)
	return *((bool*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an bool");
    }

    unsigned Value::unsignedValue() const
    {
      if(type_ == UNSIGNED)
	return *((unsigned*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an unsigned int");
    }

    int Value::intValue() const
    {
      if(type_ == INT)
	return *((int*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an int int");
    }

    float Value::floatValue() const
    {
      float result;
      if(type_ == FLOAT)
	result = *((float*)value_);
      return result;
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a float");
    }

    double Value::doubleValue() const
    {
      double result;
      if(type_ == DOUBLE)
	result = *((double*)value_);
      return result;
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a double");
    }

    std::string Value::stringValue() const
    {
      if(type_ == STRING)
	return *((std::string*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an string");
    }

    Vector Value::vectorValue() const
    {
      if(type_ == VECTOR)
	return *((Vector*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an vector");
    }

    Matrix Value::matrixValue() const
    {
      if(type_ == MATRIX)
	return *((Matrix*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a matrix");
    }

    std::string Value::typeName(Type type)
    {
      switch(type) {
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
      case VECTOR:
	return std::string("vector");
      case MATRIX:
	return std::string("matrix");
      default:
	return std::string("unknown");
      }
    }

    std::ostream& operator<<(std::ostream& os, const Value& value)
    {
      os << "Type=" << Value::typeName(value.type_)
	 << ", value=";
      switch(value.type_) {
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
      case Value::VECTOR:
	os << value.vectorValue();
	break;
      case Value::MATRIX:
	os << value.matrixValue();
	break;
      default:
	return os;
      }
      return os;
    }

  } // namespace command
} //namespace dynamicgraph
