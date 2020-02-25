// -*- c++-mode -*-
// Copyright 2010 François Bleibel Thomas Moulard, Olivier Stasse
//

#ifndef DYNAMIC_GRAPH_SIGNAL_CASTER_HH
#define DYNAMIC_GRAPH_SIGNAL_CASTER_HH
#include <map>
#include <typeinfo>
#include <vector>

#include <boost/any.hpp>
#include <boost/format.hpp>
#include <boost/function/function1.hpp>
#include <boost/function/function2.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>

#include "dynamic-graph/exception-signal.h"
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/linear-algebra.h>

namespace dynamicgraph {
/// This singleton class allows serialization of a number of objects into
/// (disp) and from (cast) std i/o streams.
///
/// The transformation is done at run-time, i.e. SignalCaster
/// doesn't know about the type of objects it casts to.
///
/// It also allows registering of user-defined casts. A cast is
/// identified by the compiler. The mapping from a type to a
/// serialization function is dynamic, hence it is more complex than
/// a typical template-based compile-time resolve. So disp, cast and
/// trace are costly functions and should be used as such.
class DYNAMIC_GRAPH_DLLAPI SignalCaster {
public:
  virtual ~SignalCaster();
  /// Destroy the unique instance.
  static void destroy();
  /// Typedef of displayer functions that take an encapsulated 'any'
  /// object and displays, cast, or trace it on an output stream
  /// (serialization).
  typedef boost::function2<void, const boost::any &, std::ostream &>
      displayer_type;
  typedef boost::function1<boost::any, std::istringstream &> caster_type;
  typedef boost::function2<void, const boost::any &, std::ostream &>
      tracer_type;

  /// Get a reference to the unique object of the class.
  static SignalCaster *getInstance(void);
  /// Displays an object using a registered displayer function.
  void disp(const boost::any &object, std::ostream &os);
  /// Traces an object using a registered trace function.
  void trace(const boost::any &object, std::ostream &os);
  /// Casts an object using a registered cast function.
  boost::any cast(const std::type_info &, std::istringstream &iss);
  /// Registers a cast.
  void registerCast(const std::type_info &type, displayer_type displayer,
                    caster_type caster, tracer_type tracer);
  /// Unregister a cast.
  void unregisterCast(const std::type_info &type);
  /// Checks if there is a displayer registered with type_name.
  bool existsCast(const std::type_info &type) const;
  /// Return the list of type names registered.
  std::vector<std::string> listTypenames() const;

private:
  /// Container for the three cast functions.
  typedef boost::tuple<displayer_type, caster_type, tracer_type>
      cast_functions_type;

  /// \brief Retrieve cast structure from its name.
  cast_functions_type &getCast(const std::string &type_name);

  /// This map associates the typename of objects and the corresponding
  /// using boost::function with 'compatible' syntax
  std::map<std::string, cast_functions_type> functions_;

  std::map<std::string, const std::type_info *> type_info_;

private:
  explicit SignalCaster();
  /// Pointer to the unique instance of the class.
  static SignalCaster *instance_;
};

/// The SignalCast registerer class. Can be used to automatically
/// register a cast when instanced somewhere in a cpp file. Pass the
/// typeid () of the type you want to register a cast to as the first
/// argument. The code is provided here so the class does not need
/// to be exported.
class DYNAMIC_GRAPH_DLLAPI SignalCastRegisterer {
public:
  inline SignalCastRegisterer(const std::type_info &type,
                              SignalCaster::displayer_type displayer,
                              SignalCaster::caster_type caster,
                              SignalCaster::tracer_type tracer) {
    SignalCaster::getInstance()->registerCast(type, displayer, caster, tracer);
  }
};

/// Global signal cast template (helper) functions
///
/// Using these avoid using the typeid () operator and keeps the
/// implementation details hidden.
template <typename T> void signal_disp(const T &value, std::ostream &os) {
  SignalCaster::getInstance()->disp(value, os);
}

template <typename T> T signal_cast(std::istringstream &iss) {
  return boost::any_cast<T>(SignalCaster::getInstance()->cast(typeid(T), iss));
}

/// Template class used to display a signal value.
template <typename T> struct signal_trace {
inline static void run(const T &value, std::ostream &os) { os << value << '\n'; }
};

/// Template specialization of signal_trace for Eigen objects
template <typename Derived> struct signal_trace<Eigen::DenseBase<Derived> > {
inline static void run(const Eigen::DenseBase<Derived> &value, std::ostream &os) {
  static const Eigen::IOFormat row_format (Eigen::StreamPrecision,
      Eigen::DontAlignCols, ", ", ", ", "", "", "", "\n");
  os << value.format(row_format);
}
};

} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_SIGNAL_CASTER_HH
