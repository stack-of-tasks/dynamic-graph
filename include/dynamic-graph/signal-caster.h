// -*- c++-mode -*-
// Copyright 2010 François Bleibel Thomas Moulard, Olivier Stasse
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

#ifndef DYNAMIC_GRAPH_SIGNAL_CASTER_HH
# define DYNAMIC_GRAPH_SIGNAL_CASTER_HH
# include <map>
# include <typeinfo>
# include <iostream>

# include <boost/any.hpp>
# include <boost/format.hpp>
# include <boost/function/function1.hpp>
# include <boost/function/function2.hpp>
# include <boost/lexical_cast.hpp>
# include <boost/tuple/tuple.hpp>

# include <dynamic-graph/dynamic-graph-api.h>
# include "dynamic-graph/exception-signal.h"

namespace dynamicgraph
{
  /// This class allows serialization of a number of objects into
  /// (disp) and from (cast) std i/o streams.
  ///
  /// The transformation is done at run-time, i.e. SignalCaster
  /// doesn't know about the type of objects it casts to.
  ///
  /// It also allows registering of user-defined casts. A cast is
  /// identified by the compiler The mapping from a type to a
  /// serialization function is dynamic, hence it is more complex than
  /// a typical template-based compile-time resolve. So disp, cast and
  /// trace are costly functions and should be used as such.
  class DYNAMIC_GRAPH_DLLAPI SignalCaster
  {
  public:
    explicit SignalCaster  ();
    virtual ~SignalCaster  ();

    /// Typedef of displayer functions that take an encapsulated 'any'
    /// object and displays, cast, or trace it on an output stream
    /// (serialization).
    typedef boost::function2<void, const boost::any&, std::ostream&>
      displayer_type;
    typedef boost::function1<boost::any, std::istringstream&> caster_type;
    typedef boost::function2<void, const boost::any&, std::ostream&>
      tracer_type;

    /// Displays an object using a registered displayer function.
    void disp (const boost::any& object, std::ostream& os);
    /// Traces an object using a registered trace function.
    void trace (const boost::any& object, std::ostream& os);
    /// Casts an object using a registered cast function.
    boost::any cast (const std::type_info&, std::istringstream& iss);
    /// Registers a cast.
    void registerCast (const std::type_info& type, displayer_type displayer,
		       caster_type caster, tracer_type tracer);
    /// Unregister a cast.
    void unregisterCast (const std::type_info& type);
    /// Checks if there is a displayer registered with type_name.
    bool existsCast (const std::type_info& type) const;

  private:
    /// Container for the three cast functions.
    typedef boost::tuple<displayer_type, caster_type, tracer_type>
      cast_functions_type;

    /// \brief Retrieve cast structure from its name.
    cast_functions_type& getCast (const std::string& type_name);

    /// This map associates the typename of objects and the corresponding
    /// using boost::function with 'compatible' syntax
    std::map<std::string, cast_functions_type> functions_;
  };

  /// The library-wide instance of SignalCaster
  extern DYNAMIC_GRAPH_DLLAPI SignalCaster g_caster;

  ///The SignalCast registerer class. Can be used to automatically
  /// register a cast when instanced somewhere in a cpp file. Pass the
  /// typeid () of the type you want to register a cast to as the first
  /// argument. The code is provided here so the class does not need
  /// to be exported.
  class DYNAMIC_GRAPH_DLLAPI SignalCastRegisterer
  {
  public:
    inline SignalCastRegisterer (const std::type_info& type,
				 SignalCaster::displayer_type displayer,
				 SignalCaster::caster_type caster,
				 SignalCaster::tracer_type tracer)
    {
      g_caster.registerCast(type, displayer, caster, tracer);
    }
  };

  /// This class can be used to register default casts, i.e. casts
  /// already supported by the object to an std::iostream through the
  /// operators >> and << .
  template<typename T>
  class DefaultCastRegisterer : public SignalCastRegisterer
  {
  public:
    DefaultCastRegisterer  ()
      : SignalCastRegisterer (typeid(T), disp, cast, trace)
    {}

    static boost::any cast (std::istringstream& iss);

    static void disp (const boost::any& object, std::ostream& os)
    {
      os << boost::any_cast<T> (object) << std::endl;
    }

    static void trace (const boost::any& object, std::ostream& os)
    {
      disp(object,os);
    }
  };


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
	return std::numeric_limits<double>::quiet_NaN  ();
      else if (tmp == "inf" || tmp == "+inf")
	return std::numeric_limits<double>::infinity  ();
      else if (tmp == "-inf")
	return -1. * std::numeric_limits<double>::infinity  ();

      try
	{
	  return boost::lexical_cast<double> (tmp);
	}
      catch (boost::bad_lexical_cast&)
	{
	  boost::format fmt ("failed to serialize %s (to double)");
	  fmt % tmp;
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str  ());
	}
    }

  template <typename T>
  boost::any
  DefaultCastRegisterer<T>::cast (std::istringstream& iss)
    {
      T inst;
      iss >> inst;
      if (iss.fail  ())
	{
	  boost::format fmt ("failed to serialize %s ");
	  fmt % iss.str  ();
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str  ());
	}
      return inst;
    }



  /// Global signal cast template (helper) functions
  ///
  /// Using these avoid using the typeid () operator and keeps the
  /// implementation details hidden.
  template<typename T>
  void signal_disp (const T& value, std::ostream& os)
  {
    g_caster.disp(value, os);
  }

  template<typename T>
  T signal_cast(std::istringstream& iss)
  {
    return boost::any_cast<T>(g_caster.cast(typeid(T), iss));
  }

  template<typename T>
  void signal_trace (const T& value, std::ostream& os)
  {
    g_caster.trace(value, os);
  }
} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_SIGNAL_CASTER_HH
