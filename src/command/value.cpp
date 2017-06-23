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
    EitherType::operator Eigen::MatrixXd() const
    {
      return value_->matrixXdValue();
    }

    EitherType::operator Eigen::Matrix4d() const
    {
      return value_->matrix4dValue();
    }

    void Value::deleteValue ()
    {
      switch(type_) {
      case BOOL:
	delete(const bool*)value_;
	break;
      case UNSIGNED:
	delete(const unsigned*)value_;
	break;
      case INT:
	delete(const int*)value_;
	break;
      case FLOAT:
	delete(const float*)value_;
	break;
      case DOUBLE:
	delete(const double*)value_;
	break;
      case STRING:
	delete(const std::string*)value_;
	break;
      case VECTOR:
	delete(const Vector*)value_;
	break;
      case MATRIX:
	delete(const Eigen::MatrixXd*)value_;
	break;
      case MATRIX4D:
	delete(const Eigen::Matrix4d*)value_;
	break;
      default:;
      }
    }

    Value::~Value()
    {
      deleteValue ();
    }

    Value::Value(const bool& value) : type_(BOOL), value_(new bool(value))
    {
    }
    Value::Value(const unsigned& value) : type_(UNSIGNED),
					  value_(new unsigned(value))
    {
    }
    Value::Value(const int& value) : type_(INT),
				     value_(new int(value))
    {
    }
    Value::Value(const float& value) : type_(FLOAT),
				       value_(new float(value))
    {
    }
    Value::Value(const double& value) : type_(DOUBLE),
					value_(new double(value))
    {
    }
    Value::Value(const std::string& value) : type_(STRING),
					     value_(new std::string(value))
    {
    }
    Value::Value(const Vector& value) : type_(VECTOR),
					value_(new Vector(value))
    {
    }
    Value::Value(const Eigen::MatrixXd& value) : type_(MATRIX),
						 value_(new Eigen::MatrixXd(value))
    {
    }
    Value::Value(const Eigen::Matrix4d& value) : type_(MATRIX4D),
						 value_(new Eigen::Matrix4d(value))
    {
    }


    Value::Value(const Value& value) : type_(value.type_),
				       value_(copyValue(value))
    {
    }

    void* copyValue(const Value& value)
    {
      void* copy;
      switch(value.type()) {
      case Value::NONE:
	copy = NULL;
      case Value::BOOL:
	copy = new bool(value.boolValue());
	break;
      case Value::UNSIGNED:
	copy = new unsigned(value.unsignedValue());
	break;
      case Value::INT:
	copy = new int(value.intValue());
	break;
      case Value::FLOAT:
	copy = new float(value.floatValue());
	break;
      case Value::DOUBLE:
	copy = new double(value.doubleValue());
	break;
      case Value::STRING:
	copy = new std::string(value.stringValue());
	break;
      case Value::VECTOR:
	copy = new Vector(value.vectorValue());
	break;
      case Value::MATRIX:
	copy = new Eigen::MatrixXd(value.matrixXdValue());
	break;
      case Value::MATRIX4D:
	copy = new Eigen::Matrix4d(value.matrix4dValue());
	break;
      default:
	abort();
      }
      return copy;
    }

    Value::Value() : type_(NONE), value_(NULL)
    {
    }

    Value Value::operator=(const Value& value)
    {
      if (&value != this) {
	if(value_ != 0x0)
	  deleteValue ();
	type_ = value.type_;
	void** ptValue = const_cast<void**>(&value_);
	*ptValue = copyValue(value);
      }
      return *this;
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
	return *((const bool*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an bool");
    }

    unsigned Value::unsignedValue() const
    {
      if(type_ == UNSIGNED)
	return *((const unsigned*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an unsigned int");
    }

    int Value::intValue() const
    {
      if(type_ == INT)
	return *((const int*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an int int");
    }

    float Value::floatValue() const
    {
      float result;
      if(FLOAT != type_)
	throw ExceptionAbstract(ExceptionAbstract::TOOLS,
				"value is not a float");
      result = *((const float*)value_);
      return result;
    }

    double Value::doubleValue() const
    {
      double result;
      if(DOUBLE != type_)
	throw ExceptionAbstract(ExceptionAbstract::TOOLS,
				"value is not a double");
      result = *((const double*)value_);
      return result;
    }

    std::string Value::stringValue() const
    {
      if(type_ == STRING)
	return *((const std::string*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an string");
    }

    Vector Value::vectorValue() const
    {
      if(type_ == VECTOR)
	return *((const Vector*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not an vector");
    }

    Eigen::MatrixXd Value::matrixXdValue() const
    {
      if(type_ == MATRIX)
	return *((const Eigen::MatrixXd*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a Eigen matrixXd");
    }

    Eigen::Matrix4d Value::matrix4dValue() const
    {
      if(type_ == MATRIX4D)
	return *((const Eigen::Matrix4d*)value_);
      throw ExceptionAbstract(ExceptionAbstract::TOOLS,
			      "value is not a Eigen matrix4d");
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
	return std::string("matrixXd");
      case MATRIX4D:
	return std::string("matrix4d");
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
	os << value.matrixXdValue();
	break;
      case Value::MATRIX4D:
	os << value.matrix4dValue();
	break;
      default:
	return os;
      }
      return os;
    }

    template<> const Value::Type ValueHelper<bool>::TypeID = Value::BOOL;
    template<> const Value::Type ValueHelper<unsigned>::TypeID = Value::UNSIGNED;
    template<> const Value::Type ValueHelper<int>::TypeID = Value::INT;
    template<> const Value::Type ValueHelper<float>::TypeID = Value::FLOAT;
    template<> const Value::Type ValueHelper<double>::TypeID = Value::DOUBLE;
    template<> const Value::Type ValueHelper<std::string>::TypeID = Value::STRING;
    template<> const Value::Type ValueHelper<Vector>::TypeID = Value::VECTOR;
    template<> const Value::Type ValueHelper<Eigen::MatrixXd>::TypeID = Value::MATRIX;
    template<> const Value::Type ValueHelper<Eigen::Matrix4d>::TypeID = Value::MATRIX4D;

  } // namespace command
} //namespace dynamicgraph
