//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#include "dynamic-graph/value.h"

#include "dynamic-graph/exception-abstract.h"

namespace dynamicgraph {
namespace command {

static void *copyValue(const Value &value);

EitherType::EitherType(const Value &value) : value_(new Value(value)) {}

EitherType::~EitherType() {
  delete value_;
  value_ = NULL;
}

EitherType::operator bool() const { return value_->boolValue(); }
EitherType::operator std::uint32_t() const { return value_->unsignedValue(); }
EitherType::operator std::uint64_t() const { return value_->unsignedlongintValue(); }
EitherType::operator std::int32_t() const { return value_->intValue(); }
EitherType::operator std::int64_t() const { return value_->longintValue(); }
EitherType::operator float() const { return value_->floatValue(); }
EitherType::operator double() const { return value_->doubleValue(); }
EitherType::operator std::string() const { return value_->stringValue(); }
EitherType::operator Vector() const { return value_->vectorValue(); }
EitherType::operator Eigen::MatrixXd() const { return value_->matrixXdValue(); }

EitherType::operator Eigen::Matrix4d() const { return value_->matrix4dValue(); }
EitherType::operator Values() const { return value_->valuesValue(); }

void Value::deleteValue() {
  switch (type_) {
    case BOOL:
      delete (const bool *)value_;
      break;
    case UNSIGNED:
      delete (const std::uint32_t *)value_;
      break;
    case UNSIGNEDLONGINT:
      delete (const std::uint64_t *)value_;
      break;
    case INT:
      delete (const std::int32_t *)value_;
      break;
    case LONGINT:
      delete (const std::int64_t *)value_;
      break;
    case FLOAT:
      delete (const float *)value_;
      break;
    case DOUBLE:
      delete (const double *)value_;
      break;
    case STRING:
      delete (const std::string *)value_;
      break;
    case VECTOR:
      delete (const Vector *)value_;
      break;
    case MATRIX:
      delete (const Eigen::MatrixXd *)value_;
      break;
    case MATRIX4D:
      delete (const Eigen::Matrix4d *)value_;
      break;
    case VALUES:
      delete (const Values *)value_;
      break;
    case NONE: /* Equivalent to void */
      break;
    default:
      throw "Value::deleteValue : Undefined type";
      ;
  }
}

Value::~Value() { deleteValue(); }

Value::Value(const bool &value) : type_(BOOL), value_(new bool(value)) {}
Value::Value(const std::uint32_t &value)
    : type_(UNSIGNED), value_(new std::uint32_t(value)) {}
Value::Value(const std::uint64_t &value)
    : type_(UNSIGNEDLONGINT), value_(new std::uint64_t(value)) {}
Value::Value(const std::int32_t &value) : type_(INT), value_(new std::int32_t(value)) {}
Value::Value(const std::int64_t &value)
    : type_(LONGINT), value_(new int64_t(value)) {}
Value::Value(const float &value) : type_(FLOAT), value_(new float(value)) {}
Value::Value(const double &value) : type_(DOUBLE), value_(new double(value)) {}
Value::Value(const std::string &value)
    : type_(STRING), value_(new std::string(value)) {}
Value::Value(const Vector &value) : type_(VECTOR), value_(new Vector(value)) {}
Value::Value(const Eigen::MatrixXd &value)
    : type_(MATRIX), value_(new Eigen::MatrixXd(value)) {}
Value::Value(const Eigen::Matrix4d &value)
    : type_(MATRIX4D), value_(new Eigen::Matrix4d(value)) {}
Value::Value(const Values &value) : type_(VALUES), value_(new Values(value)) {}

Value::Value(const Value &value)
    : type_(value.type_), value_(copyValue(value)) {}

void *copyValue(const Value &value) {
  void *copy;
  switch (value.type()) {
    case Value::NONE:
      copy = NULL;
      break;
    case Value::BOOL:
      copy = new bool(value.boolValue());
      break;
    case Value::UNSIGNED:
      copy = new unsigned(value.unsignedValue());
      break;
    case Value::UNSIGNEDLONGINT:
      copy = new unsigned long int(value.unsignedlongintValue());
      break;
    case Value::INT:
      copy = new int(value.intValue());
      break;
    case Value::LONGINT:
      copy = new long int(value.longintValue());
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
    case Value::VALUES:
      copy = new Values(value.valuesValue());
      break;
    default:
      abort();
  }
  return copy;
}

Value::Value() : type_(NONE), value_(NULL) {}

Value Value::operator=(const Value &value) {
  if (&value != this) {
    if (value_ != 0x0) deleteValue();
    type_ = value.type_;
    void **ptValue = const_cast<void **>(&value_);
    *ptValue = copyValue(value);
  }
  return *this;
}

bool Value::operator==(const Value &other) const {
  if (type_ != other.type_) return false;
  switch (type_) {
    case Value::BOOL:
      return boolValue() == other.boolValue();
    case Value::UNSIGNED:
      return unsignedValue() == other.unsignedValue();
    case Value::UNSIGNEDLONGINT:
      return unsignedlongintValue() == other.unsignedlongintValue();
    case Value::INT:
      return intValue() == other.intValue();
    case Value::DOUBLE:
      return doubleValue() == other.doubleValue();
    case Value::FLOAT:
      return floatValue() == other.floatValue();
    case Value::STRING:
      return stringValue() == other.stringValue();
    case Value::VECTOR:
      return vectorValue() == other.vectorValue();
    case Value::MATRIX:
      return matrixXdValue() == other.matrixXdValue();
    case Value::MATRIX4D:
      return matrix4dValue() == other.matrix4dValue();
    case Value::VALUES:
      return constValuesValue() == other.constValuesValue();
    case Value::NONE:
      break;
    default:
      break;
  }
  return false;
}

const EitherType Value::value() const { return EitherType(*this); }

Value::Type Value::type() const { return type_; }

bool Value::boolValue() const {
  if (type_ == BOOL) return *((const bool *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS, "value is not an bool");
}

std::uint32_t Value::unsignedValue() const {
  if (type_ == UNSIGNED) return *((const std::uint32_t *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS,
                          "value is not an unsigned int");
}

std::uint64_t Value::unsignedlongintValue() const {
  if (type_ == UNSIGNEDLONGINT) return *((const std::uint64_t *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS,
                          "value is not an unsigned long int");
}

std::int64_t Value::longintValue() const {
  if (type_ == LONGINT) return *((const std::int64_t *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS, "value is not an long int");
}

std::int32_t Value::intValue() const {
  if (type_ == INT) return *((const std::int32_t *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS, "value is not an int");
}

float Value::floatValue() const {
  float result;
  if (FLOAT != type_)
    throw ExceptionAbstract(ExceptionAbstract::TOOLS, "value is not a float");
  result = *((const float *)value_);
  return result;
}

double Value::doubleValue() const {
  double result;
  if (DOUBLE != type_)
    throw ExceptionAbstract(ExceptionAbstract::TOOLS, "value is not a double");
  result = *((const double *)value_);
  return result;
}

std::string Value::stringValue() const {
  if (type_ == STRING) return *((const std::string *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS, "value is not an string");
}

Vector Value::vectorValue() const {
  if (type_ == VECTOR) return *((const Vector *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS, "value is not an vector");
}

Eigen::MatrixXd Value::matrixXdValue() const {
  if (type_ == MATRIX) return *((const Eigen::MatrixXd *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS,
                          "value is not a Eigen matrixXd");
}

Eigen::Matrix4d Value::matrix4dValue() const {
  if (type_ == MATRIX4D) return *((const Eigen::Matrix4d *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS,
                          "value is not a Eigen matrix4d");
}

Values Value::valuesValue() const {
  if (type_ == VALUES) return *((const Values *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS,
                          "value is not a vector of Value");
}

const Values &Value::constValuesValue() const {
  if (type_ == VALUES) return *((const Values *)value_);
  throw ExceptionAbstract(ExceptionAbstract::TOOLS,
                          "value is not a vector of Value");
}

std::string Value::typeName(Type type) {
  switch (type) {
    case BOOL:
      return std::string("bool");
    case UNSIGNED:
      return std::string("unsigned int");
    case UNSIGNEDLONGINT:
      return std::string("unsigned long int");
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
    case VALUES:
      return std::string("values");
    default:
      return std::string("unknown");
  }
}

std::ostream &operator<<(std::ostream &os, const Value &value) {
  os << "Type=" << Value::typeName(value.type_) << ", value=";
  switch (value.type_) {
    case Value::BOOL:
      os << value.boolValue();
      break;
    case Value::UNSIGNED:
      os << value.unsignedValue();
      break;
    case Value::UNSIGNEDLONGINT:
      os << value.unsignedlongintValue();
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
    case Value::VALUES: {
      const std::vector<Value> &vals = value.constValuesValue();
      os << "[ ";
      for (std::size_t i = 0; i < vals.size(); ++i)
        os << "Value(" << vals[i] << "), ";
      os << "]";
    } break;
    default:
      return os;
  }
  return os;
}

template <>
const Value::Type ValueHelper<bool>::TypeID = Value::BOOL;
template <>
const Value::Type ValueHelper<std::uint32_t>::TypeID = Value::UNSIGNED;
template <>
const Value::Type ValueHelper<std::uint64_t>::TypeID = Value::UNSIGNEDLONGINT;
template <>
const Value::Type ValueHelper<std::int32_t>::TypeID = Value::INT;
template <>
const Value::Type ValueHelper<std::int64_t>::TypeID = Value::LONGINT;
template <>
const Value::Type ValueHelper<float>::TypeID = Value::FLOAT;
template <>
const Value::Type ValueHelper<double>::TypeID = Value::DOUBLE;
template <>
const Value::Type ValueHelper<std::string>::TypeID = Value::STRING;
template <>
const Value::Type ValueHelper<Vector>::TypeID = Value::VECTOR;

template <>
const Value::Type ValueHelper<Eigen::MatrixXd>::TypeID = Value::MATRIX;
template <>
const Value::Type ValueHelper<Eigen::Matrix4d>::TypeID = Value::MATRIX4D;
template <>
const Value::Type ValueHelper<Values>::TypeID = Value::VALUES;

}  // namespace command
}  // namespace dynamicgraph
