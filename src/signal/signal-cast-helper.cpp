// -*- c++-mode -*-
// Copyright 2010 François Bleibel
// Thomas Moulard,
// Olivier Stasse,
// Nicolas Mansard
//

#include <algorithm>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lambda/bind.hpp>
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/linear-algebra.h>
#include <dynamic-graph/signal-cast-helper.h>
#include <dynamic-graph/signal-caster.h>
#include <exception>
#include <sstream>
#include <string>

namespace dynamicgraph {

// Define a custom implementation of the DefaultCastRegisterer
// to workaround the limitations of the stream based approach.
// When dealing with double: displaying a double on a stream
// is *NOT* the opposite of reading a double from a stream.
//
// In practice, it means that there is no way to read
// a NaN, +inf, -inf from a stream!
//
// To workaround this problem, parse special values manually
// (the strings used are the one produces by displaying special
// values on a stream).

template <>
inline boost::any DefaultCastRegisterer<double>::cast(std::istringstream &iss) {
  std::string tmp;
  iss >> tmp;

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

/* Specialize Matrix and Vector traces. */

template <>
void DefaultCastRegisterer<dynamicgraph::Vector>::trace(
    const boost::any &object, std::ostream &os) {
  const dynamicgraph::Vector &v = boost::any_cast<dynamicgraph::Vector>(object);
  for (int i = 0; i < v.size(); ++i) {
    os << "\t" << v(i);
  }
}

template <>
void DefaultCastRegisterer<dynamicgraph::Matrix>::trace(
    const boost::any &object, std::ostream &os) {
  const dynamicgraph::Matrix &m = boost::any_cast<dynamicgraph::Matrix>(object);
  for (int i = 0; i < m.rows(); ++i)
    for (int j = 0; j < m.cols(); ++j) {
      os << "\t" << m(i, j);
    }
}

// for std::string, do not check failure. If input stream contains an
// empty string, iss.fail() returns true and an exception is thrown
template <>
inline boost::any DefaultCastRegisterer<std::string>::cast
(std::istringstream &iss)
{
  std::string inst ("");
  iss >> inst;
  return inst;
}

// for std::string, do not add std::endl at the end of the stream.
template <>
inline void DefaultCastRegisterer<std::string>::disp
(const boost::any &object, std::ostream &os)
{
  os << boost::any_cast<std::string>(object);
}

/// Registers useful casts
namespace {
DefaultCastRegisterer<double> double_reg;
DefaultCastRegisterer<int> int_reg;
DefaultCastRegisterer<unsigned int> uint_reg;
DefaultCastRegisterer<bool> bool_reg;
DefaultCastRegisterer<dynamicgraph::Vector> vectorCastRegisterer;
DefaultCastRegisterer<dynamicgraph::Matrix> matrixCastRegisterer;
DefaultCastRegisterer<boost::posix_time::ptime> ptimeCastRegisterer;
DefaultCastRegisterer<std::string> stringCastRegisterer;
} // end of anonymous namespace.

} // namespace dynamicgraph
