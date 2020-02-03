//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#ifndef DYNAMIC_GRAPH_VALUE_H
#define DYNAMIC_GRAPH_VALUE_H

#include "dynamic-graph/dynamic-graph-api.h"
#include <cassert>
#include <dynamic-graph/linear-algebra.h>
#include <string>
#include <typeinfo>
#include <vector>

namespace dynamicgraph {
namespace command {
class Value;
typedef std::vector<Value> Values;

class DYNAMIC_GRAPH_DLLAPI EitherType {
public:
  EitherType(const Value &value);
  ~EitherType();
  operator bool() const;
  operator unsigned() const;
  operator int() const;
  operator float() const;
  operator double() const;
  operator std::string() const;
  operator Vector() const;
  operator Eigen::MatrixXd() const;
  operator Eigen::Matrix4d() const;
  operator Values() const;

private:
  const Value *value_;
};

class DYNAMIC_GRAPH_DLLAPI Value {
public:
  enum Type {
    NONE,
    BOOL,
    UNSIGNED,
    INT,
    FLOAT,
    DOUBLE,
    STRING,
    VECTOR,
    MATRIX,
    MATRIX4D,
    VALUES,
    NB_TYPES
  };
  ~Value();
  void deleteValue();
  explicit Value(const bool &value);
  explicit Value(const unsigned &value);
  explicit Value(const int &value);
  explicit Value(const float &value);
  explicit Value(const double &value);
  explicit Value(const std::string &value);
  explicit Value(const Vector &value);
  explicit Value(const Eigen::MatrixXd &value);
  explicit Value(const Eigen::Matrix4d &value);
  explicit Value(const Values &value);
  /// Copy constructor
  Value(const Value &value);
  // Construct an empty value (None)
  explicit Value();
  // operator assignement
  Value operator=(const Value &value);
  // Equality operator
  bool operator==(const Value &other) const;
  /// Return the type of the value
  Type type() const;

  /// Return the value as a castable value into the approriate type
  ///
  /// For instance,
  /// \code
  /// Value v1(5.0); // v1 is of type double
  /// Value v2(3);   // v2 is of type int
  /// double x1 = v1.value();
  /// double x2 = v2.value();
  /// \endcode
  /// The first assignment will succeed, while the second one will throw
  /// an exception.
  const EitherType value() const;
  /// Return the name of the type
  static std::string typeName(Type type);

  /// Output in a stream
  DYNAMIC_GRAPH_DLLAPI friend std::ostream &operator<<(std::ostream &os,
                                                       const Value &value);

public:
  friend class EitherType;
  bool boolValue() const;
  unsigned unsignedValue() const;
  int intValue() const;
  float floatValue() const;
  double doubleValue() const;
  std::string stringValue() const;
  Vector vectorValue() const;
  Eigen::MatrixXd matrixXdValue() const;
  Eigen::Matrix4d matrix4dValue() const;
  Values valuesValue() const;
  const Values &constValuesValue() const;
  Type type_;
  const void *const value_;
};

/* ---- HELPER ---------------------------------------------------------- */
// Note: to ensure the WIN32 compatibility, it is necessary to export
// the template specialization. Also, it is forbidden to do the template
// specialization declaration in the header file, for the same reason.
template <typename T> struct DYNAMIC_GRAPH_DLLAPI ValueHelper {
  static const Value::Type TypeID;
};
} // namespace command
} // namespace dynamicgraph

#endif // DYNAMIC_GRAPH_VALUE_H
