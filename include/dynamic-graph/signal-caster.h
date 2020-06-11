// -*- c++-mode -*-
// Copyright 2010 François Bleibel Thomas Moulard, Olivier Stasse
//

#ifndef DYNAMIC_GRAPH_SIGNAL_CASTER_HH
#define DYNAMIC_GRAPH_SIGNAL_CASTER_HH
#include <map>
#include <vector>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "dynamic-graph/exception-signal.h"
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/linear-algebra.h>
#include <dynamic-graph/eigen-io.h>

namespace dynamicgraph {

/// Inherit from this class if you want to keep default implementation for some
/// functions.
template <typename T> struct signal_io_base {
/// serialize a signal value.
inline static void disp (const T &value, std::ostream &os) { os << value; }
/// deserialize a signal value.
inline static T cast (std::istringstream &is) {
  T inst;
  is >> inst;
  if (is.fail()) {
    throw ExceptionSignal(ExceptionSignal::GENERIC,
        "failed to serialize " + is.str());
  }
  return inst;
}
/// write a signal value to log file
inline static void trace(const T &value, std::ostream &os) { os << value; }
};

/// Inherit from this class if tracing is not implemented for a given type.
template <typename T> struct signal_io_unimplemented {
inline static void disp (const T &, std::ostream &) {
  throw std::logic_error("this disp is not implemented.");
}
inline static T cast (std::istringstream &) {
  throw std::logic_error("this cast is not implemented.");
}
inline static void trace(const T &, std::ostream &) {
  throw std::logic_error("this trace is not implemented.");
}
};

/// Class used for I/O operations in Signal<T,Time>
template <typename T> struct signal_io : signal_io_base<T> {};

/// Template specialization of signal_disp for Eigen objects
template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
struct signal_io<Eigen::Matrix< _Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols > >
: signal_io_base <Eigen::Matrix< _Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols > >
{
typedef Eigen::Matrix< _Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols > matrix_type;

inline static void disp(const matrix_type &value, std::ostream &os) {
  static const Eigen::IOFormat row_format (Eigen::StreamPrecision,
      Eigen::DontAlignCols, " ", " ", "", "", "", "");
  os << value.format(row_format);
}

inline static void trace(const matrix_type &value, std::ostream &os) {
  static const Eigen::IOFormat row_format (Eigen::StreamPrecision,
      Eigen::DontAlignCols, "\t", "\t", "", "", "", "");
  os << value.format(row_format);
}
};

/// Template specialization of signal_io for Eigen quaternion objects
template<typename _Scalar, int _Options>
struct signal_io<Eigen::Quaternion< _Scalar, _Options> >
: signal_io_base<Eigen::Quaternion< _Scalar, _Options> >
{
typedef Eigen::Quaternion< _Scalar, _Options> quat_type;
typedef Eigen::Matrix< _Scalar, 4, 1, _Options> matrix_type;

inline static void disp(const quat_type &value, std::ostream &os) {
  signal_io<matrix_type>::run(value.coeffs());
}

inline static quat_type cast (std::istringstream &is) {
  return quat_type(signal_io<matrix_type>::cast(is));
}

inline static void trace(const quat_type &value, std::ostream &os) {
  signal_io<matrix_type>::trace(value.coeffs());
}
};

/// Template specialization of signal_io for std::string.
/// Do not print '\n' at the end.
template <> struct signal_io<std::string> : signal_io_base<std::string>
{
inline static std::string cast (std::istringstream &iss) { return iss.str(); }
};

/// Template specialization of signal_io for double
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
template <> struct signal_io<double> : signal_io_base<double> {
inline static double cast (std::istringstream &iss) {
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

} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_SIGNAL_CASTER_HH
