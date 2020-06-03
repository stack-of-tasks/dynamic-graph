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
inline static void run(const T &value, std::ostream &os) { os << value; }
};

/// Template specialization of signal_trace for Eigen objects
template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
struct signal_trace<Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> > {
inline static void run(const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &value, std::ostream &os) {
  static const Eigen::IOFormat row_format (Eigen::StreamPrecision,
      Eigen::DontAlignCols, "\t", "\t", "", "", "", "");
  os << value.format(row_format);
}
};

} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_SIGNAL_CASTER_HH
