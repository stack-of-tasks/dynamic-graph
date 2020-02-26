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
#include <dynamic-graph/eigen-io.h>

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
  typedef boost::function1<boost::any, std::istringstream &> caster_type;

  /// Get a reference to the unique object of the class.
  static SignalCaster *getInstance(void);
  /// Casts an object using a registered cast function.
  boost::any cast(const std::type_info &, std::istringstream &iss);
  /// Registers a cast.
  void registerCast(const std::type_info &type,
                    caster_type caster);
  /// Unregister a cast.
  void unregisterCast(const std::type_info &type);
  /// Checks if there is a displayer registered with type_name.
  bool existsCast(const std::type_info &type) const;
  /// Return the list of type names registered.
  std::vector<std::string> listTypenames() const;

private:
  /// Container for the three cast functions.
  typedef boost::tuple<caster_type>
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
                              SignalCaster::caster_type caster) {
    SignalCaster::getInstance()->registerCast(type, caster);
  }
};

/// Template class used to serialize a signal value.
template <typename T> struct signal_disp {
inline static void run (const T &value, std::ostream &os) { os << value << '\n'; }
};

/// Template specialization of signal_disp for Eigen objects
template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
struct signal_disp<Eigen::Matrix< _Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols > > {
inline static void run(const Eigen::Matrix< _Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols > &value, std::ostream &os) {
  static const Eigen::IOFormat row_format (Eigen::StreamPrecision,
      Eigen::DontAlignCols, " ", " ", "", "", "", "");
  os << value.format(row_format);
}
};

/// Template specialization of signal_disp for std::string.
/// Do not print '\n' at the end.
template <> struct signal_disp<std::string> {
inline static void run (const std::string &value, std::ostream &os) { os << value; }
};

/// Template class used to deserialize a signal value (reverse of signal_disp).
template <typename T> struct signal_cast {
inline static T run (std::istringstream &iss) {
  T inst;
  iss >> inst;
  if (iss.fail()) {
    throw ExceptionSignal(ExceptionSignal::GENERIC,
        "failed to serialize " + iss.str());
  }
  return inst;
}
};

/// Template specialization of signal_cast for std::string.
template <> struct signal_cast<std::string> {
inline static std::string run (std::istringstream &iss) { return iss.str(); }
};

/// Template specialization of signal_cast for double
/// to workaround the limitations of the stream based approach.
///
/// When dealing with double: displaying a double on a stream
/// is *NOT* the opposite of reading a double from a stream.
///
/// In practice, it means that there is no way to read
/// a NaN, +inf, -inf from a stream!
///
/// To workaround this problem, parse special values manually
/// (the strings used are the one produces by displaying special
/// values on a stream).
template <> struct signal_cast<double> {
inline static double run (std::istringstream &iss) {
  std::string tmp (iss.str());

  if (tmp == "nan")
    return std::numeric_limits<double>::quiet_NaN();
  else if (tmp == "inf" || tmp == "+inf")
    return std::numeric_limits<double>::infinity();
  else if (tmp == "-inf")
    return -1. * std::numeric_limits<double>::infinity();

  try {
    return boost::lexical_cast<double>(tmp);
  } catch (boost::bad_lexical_cast &) {
    boost::format fmt("failed to serialize %s (to double)");
    fmt % tmp;
    throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
  }
}
};

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
