// -*- c++-mode -*-
// Copyright 2010 François Bleibel Thomas Moulard, Olivier Stasse, Nicolas Mansard
//
// This file is part of dynamic-graph.
// dynamic-graph is free software:
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <boost/date_time/posix_time/posix_time.hpp>
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
    for( int i=0;i<v.size();++i )
      { os << "\t" << v(i); }
  }
  template <>
  void
  DefaultCastRegisterer<dynamicgraph::Matrix>::
  trace(const boost::any& object, std::ostream& os)
  {
    const dynamicgraph::Matrix & m = boost::any_cast<dynamicgraph::Matrix> (object);
    for(int i=0;i<m.rows();++i )
      for(int j=0;j<m.cols();++j )
	{ os << "\t" << m(i,j); }
  }


  /// Registers useful casts
  namespace
  {
    DefaultCastRegisterer<double> double_reg;
    DefaultCastRegisterer<int> int_reg;
    DefaultCastRegisterer<unsigned int> uint_reg;
    DefaultCastRegisterer<bool> bool_reg;
    DefaultCastRegisterer<dynamicgraph::Vector> vectorCastRegisterer;
    DefaultCastRegisterer<dynamicgraph::Matrix> matrixCastRegisterer;
    DefaultCastRegisterer <boost::posix_time::ptime> ptimeCastRegisterer;
  } // end of anonymous namespace.

} // namespace dynamicgraph
