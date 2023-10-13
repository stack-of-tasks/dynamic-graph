//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#ifndef DYNAMIC_GRAPH_COMMAND_SETTER_T_CPP
#define DYNAMIC_GRAPH_COMMAND_SETTER_T_CPP

#include "dynamic-graph/command-setter.h"

#include <boost/assign/list_of.hpp>
#include <sstream>

#include "dynamic-graph/linear-algebra.h"

namespace dynamicgraph {
class Entity;
namespace command {

//
// Template specialization: bool
//
template <class E>
class Setter<E, bool> : public Command {
 public:
  /// Pointer to method that sets parameter of type bool
  typedef void (E::*SetterMethod)(const bool &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, bool>::Setter(E &entity, SetterMethod setterMethod,
                        const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::BOOL), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, bool>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  bool value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: unsigned
//
template <class E>
class Setter<E, unsigned> : public Command {
 public:
  /// Pointer to method that sets parameter of type unsigned
  typedef void (E::*SetterMethod)(const unsigned &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, unsigned>::Setter(E &entity, SetterMethod setterMethod,
                            const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::UNSIGNED), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, unsigned>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  unsigned value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: unsigned long
//
template <class E>
class Setter<E, uint64_t> : public Command {
 public:
  /// Pointer to method that sets parameter of type unsigned long
  typedef void (E::*SetterMethod)(const uint64_t &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, uint64_t>::Setter(E &entity, SetterMethod setterMethod,
                            const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::UNSIGNEDLONGINT),
              docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, uint64_t>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  uint64_t value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: int
//
template <class E>
class Setter<E, int> : public Command {
 public:
  /// Pointer to method that sets parameter of type int
  typedef void (E::*SetterMethod)(const int &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, int>::Setter(E &entity, SetterMethod setterMethod,
                       const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::INT), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, int>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  int value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: int64_t
//
template <class E>
class Setter<E, int64_t> : public Command {
 public:
  /// Pointer to method that sets parameter of type int64_t
  typedef void (E::*SetterMethod)(const int64_t &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, int64_t>::Setter(E &entity, SetterMethod setterMethod,
                           const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::LONGINT), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, int64_t>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  int64_t value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: float
//
template <class E>
class Setter<E, float> : public Command {
 public:
  /// Pointer to method that sets parameter of type float
  typedef void (E::*SetterMethod)(const float &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, float>::Setter(E &entity, SetterMethod setterMethod,
                         const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::FLOAT), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, float>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  float value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: double
//
template <class E>
class Setter<E, double> : public Command {
 public:
  /// Pointer to method that sets parameter of type double
  typedef void (E::*SetterMethod)(const double &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, double>::Setter(E &entity, SetterMethod setterMethod,
                          const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::DOUBLE), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, double>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  double value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: std::string
//
template <class E>
class Setter<E, std::string> : public Command {
 public:
  /// Pointer to method that sets parameter of type std::string
  typedef void (E::*SetterMethod)(const std::string &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, std::string>::Setter(E &entity, SetterMethod setterMethod,
                               const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::STRING), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, std::string>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  std::string value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: Vector
//
template <class E>
class Setter<E, Vector> : public Command {
 public:
  /// Pointer to method that sets parameter of type Vector
  typedef void (E::*SetterMethod)(const Vector &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, Vector>::Setter(E &entity, SetterMethod setterMethod,
                          const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::VECTOR), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, Vector>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  Vector value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

//
// Template specialization: Matrix
//
template <class E>
class Setter<E, Matrix> : public Command {
 public:
  /// Pointer to method that sets parameter of type Matrix
  typedef void (E::*SetterMethod)(const Matrix &);
  /// Constructor
  Setter(E &entity, SetterMethod setterMethod, const std::string &docString);

 protected:
  virtual Value doExecute();

 private:
  SetterMethod setterMethod_;
};  // Class Setter

template <class E>
Setter<E, Matrix>::Setter(E &entity, SetterMethod setterMethod,
                          const std::string &docString)
    : Command(entity, boost::assign::list_of(Value::MATRIX), docString),
      setterMethod_(setterMethod) {}

template <class E>
Value Setter<E, Matrix>::doExecute() {
  const std::vector<Value> &values = getParameterValues();
  // Get parameter
  Matrix value = values[0].value();
  E &entity = static_cast<E &>(owner());
  (entity.*setterMethod_)(value);
  return Value();
}

}  // namespace command
}  // namespace dynamicgraph

#endif  // DYNAMIC_GRAPH_COMMAND_SETTER_T_CPP
