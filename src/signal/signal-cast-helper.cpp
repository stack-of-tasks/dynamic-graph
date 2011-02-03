// -*- c++-mode -*-
// Copyright 2010 François Bleibel Thomas Moulard, Olivier Stasse, Nicolas Mansard
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// You should have received a copy of the GNU Lesser General Public License
// along with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#include <dynamic-graph/signal-caster.h>
#include <dynamic-graph/signal-cast-helper.h>
#include <dynamic-graph/dynamic-graph-api.h>
#include <exception>
#include <boost/lambda/bind.hpp>
#include <string>
#include <sstream>
#include <algorithm>
#include <dynamic-graph/exception-signal.h>

#include <dynamic-graph/linear-algebra.h>

namespace dynamicgraph
{

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
  inline boost::any
  DefaultCastRegisterer<double>::cast (std::istringstream& iss)
    {
      std::string tmp;
      iss >> tmp;

      if (tmp == "nan")
	return std::numeric_limits<double>::quiet_NaN ();
      else if (tmp == "inf" || tmp == "+inf")
	return std::numeric_limits<double>::infinity ();
      else if (tmp == "-inf")
	return -1. * std::numeric_limits<double>::infinity ();

      try
	{
	  return boost::lexical_cast<double> (tmp);
	}
      catch (boost::bad_lexical_cast&)
	{
	  boost::format fmt ("failed to serialize %s (to double)");
	  fmt % tmp;
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str ());
	}
    }

  /* Specialize Matrix and Vector traces. */
  template <>
  void
  DefaultCastRegisterer<dynamicgraph::Vector>::
  trace(const boost::any& object, std::ostream& os)
  {
    const dynamicgraph::Vector & v = boost::any_cast<dynamicgraph::Vector> (object);
    for( unsigned int i=0;i<v.size();++i )
      { os << "\t" << v(i); }
  }
  template <>
  void
  DefaultCastRegisterer<dynamicgraph::Matrix>::
  trace(const boost::any& object, std::ostream& os)
  {
    const dynamicgraph::Matrix & m = boost::any_cast<dynamicgraph::Matrix> (object);
    for( unsigned int i=0;i<m.nbRows();++i )
      for( unsigned int j=0;j<m.nbCols();++j )
	{ os << "\t" << m(i,j); }
  }



  /// Registers useful casts
  namespace
  {
    DefaultCastRegisterer<double> double_reg;
    DefaultCastRegisterer<int> int_reg;
    DefaultCastRegisterer<unsigned int> uint_reg;
    DefaultCastRegisterer<dynamicgraph::Vector> vectorCastRegisterer;
    DefaultCastRegisterer<dynamicgraph::Matrix> matrixCastRegisterer;
  } // end of anonymous namespace.

} // namespace dynamicgraph
