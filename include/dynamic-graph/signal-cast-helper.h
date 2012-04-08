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

#ifndef DYNAMIC_GRAPH_SIGNAL_CASTER_HELPER_HH
# define DYNAMIC_GRAPH_SIGNAL_CASTER_HELPER_HH
# include <map>
# include <typeinfo>
# include <iostream>
# include <vector>

# include <boost/any.hpp>
# include <boost/format.hpp>
# include <boost/function/function1.hpp>
# include <boost/function/function2.hpp>
# include <boost/lexical_cast.hpp>
# include <boost/tuple/tuple.hpp>

# include <dynamic-graph/dynamic-graph-api.h>
# include "dynamic-graph/exception-signal.h"
# include "dynamic-graph/signal-caster.h"

namespace dynamicgraph
{

  /* --- NON GENERIC CASTER ------------------------------------------------- */

  /// This class can be used to register default casts, i.e. casts
  /// already supported by the object to an std::iostream through the
  /// operators >> and << .
  template<typename T>
  class DefaultCastRegisterer : public SignalCastRegisterer
  {
  public:
    DefaultCastRegisterer ()
      : SignalCastRegisterer (typeid(T), disp, cast, trace)
    {}

    DYNAMIC_GRAPH_DLLEXPORT
      static boost::any cast (std::istringstream& iss);

    DYNAMIC_GRAPH_DLLEXPORT
      static void disp (const boost::any& object, std::ostream& os)
    {
      os << boost::any_cast<T> (object) << std::endl;
    }

    DYNAMIC_GRAPH_DLLEXPORT
      static void trace (const boost::any& object, std::ostream& os)
    {
      disp(object,os);
    }
  };

  /// A default version of the caster, to serialize directly from
  /// std::in.
  template <typename T>
  boost::any
  DefaultCastRegisterer<T>::cast (std::istringstream& iss)
    {
      T inst;
      iss >> inst;
      if (iss.fail ())
	{
	  boost::format fmt ("failed to serialize %s ");
	  fmt % iss.str ();
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str ());
	}
      return inst;
    }


  /* --- GENERIC CASTER ----------------------------------------------------- */
  /*!
   * This class is only used to group together static functions who differ by
   * a template parameter. It is never actually instanced (the private constructor
   * makes sure of that).
   * Typical use of this class is to add the caster in the dg graph:
   *   dynamicgraph::SignalCastRegisterer sotCastRegisterer_TYPE
   *         (typeid(TYPE),
   *          SignalCast<TYPE>::disp_,
   *          SignalCast<TYPE>::cast_,
   *          SignalCast<TYPE>::trace_);

   * NMSD: I don't really understand the use of this additional class. IMHO
   * (comme on dit), it should be possible to rewrite the same-spec macros
   * using specialization of the template class DefaultCastRegisterer. No?
   */
  template< class T >
    class SignalCast
    {
    public:
      static T cast( std::istringstream& stringValue ) { throw 1;}
      static void disp( const T& t,std::ostream& os )  { throw 1;  }
      static void trace( const T& t,std::ostream& os ) { disp(t,os); }
    public:
      // adapter functions for SignalCast
      static boost::any cast_( std::istringstream& stringValue ) {
	return boost::any_cast<T>(cast(stringValue));
      }
      static void disp_( const boost::any& t,std::ostream& os )  {
	disp(boost::any_cast<T>(t), os);
      }
      static void trace_( const boost::any& t,std::ostream& os ) {
	trace(boost::any_cast<T>(t),os);
      }
    private:
      SignalCast() {}
    };

} // namespace dynamicgraph

/* -------------------------------------------------------------------------- */
/* --- MACROS --------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* Declaration macro: one instance of each class needs to be present in
 * order for casts to be registered.
 */

#define DG_SIGNAL_CAST_DECLARATION(TYPE) \
  ::dynamicgraph::SignalCastRegisterer sotCastRegisterer_##TYPE	\
    (typeid(TYPE),						\
     SignalCast<TYPE>::disp_,					\
     SignalCast<TYPE>::cast_,					\
     SignalCast<TYPE>::trace_)

#define DG_SIGNAL_CAST_DECLARATION_NAMED(TYPE,NAME)			\
  ::dynamicgraph::SignalCastRegisterer sotCastRegisterer_##NAME		\
    (typeid(TYPE),							\
     SignalCast<TYPE>::disp_,						\
     SignalCast<TYPE>::cast_,						\
     SignalCast<TYPE>::trace_)

/* Standard definition macros: the three functions can be specified
 * in the macros. To define then in the cpp, just put ';' in the args.
 */
#define DG_SIGNAL_CAST_FULL_DEFINITION(TYPE,CAST,DISP,TRACE)                          \
template<>                                                                            \
 class SignalCast<TYPE>					                              \
{                                                                                     \
public:                                                                               \
        static TYPE cast( std::istringstream& iss )         CAST                      \
	static void disp( TYPE const& t,std::ostream& os )  DISP	              \
	static void trace( TYPE const& t,std::ostream& os ) TRACE                     \
public:            		                                                      \
       DYNAMIC_GRAPH_DLLEXPORT                                                        \
	static boost::any cast_( std::istringstream& stringValue ) {           	      \
 		  return boost::any_cast<TYPE>(cast(stringValue));           	      \
	}                                                                             \
       DYNAMIC_GRAPH_DLLEXPORT                                                        \
	static void disp_( const boost::any& t,std::ostream& os )  {       	      \
	  disp(boost::any_cast<TYPE>(t), os);         				      \
	}            								      \
       DYNAMIC_GRAPH_DLLEXPORT                                                        \
	static void trace_( const boost::any& t,std::ostream& os ) {      	      \
		  trace(boost::any_cast<TYPE>(t),os);      			      \
	}          								      \
}

/* Standard definition macros: the functions <cast> and <disp> have
 * to be implemented in the cpp files. The function <trace> is
 * implemented as a proxy on <disp>.
 */
#define DG_SIGNAL_CAST_DEFINITION_HPP(TYPE)                              \
 DG_SIGNAL_CAST_FULL_DEFINITION(TYPE,;,;,{ disp(t,os); })

/* Lazy definition: <cast> and <disp> are to proxys on the standard
 * std input (>>) and output (<<). The function <trace> has to be
 * implemented in the cpp.
 */
#define DG_SIGNAL_CAST_DEFINITION_TRACE_HPP(TYPE,TRACE)                  \
 DG_SIGNAL_CAST_FULL_DEFINITION(TYPE,                                    \
 {TYPE res; iss >> res; return res; },                                   \
 { os << t <<std::endl; },                                               \
 TRACE )

/* Lazy lazy definition: the three functions are implemented as
 * proxys on std::io operation.
 */
#define DG_SIGNAL_CAST_DEFINITION(TYPE)                                  \
 DG_SIGNAL_CAST_FULL_DEFINITION(TYPE,                                    \
 {TYPE res; iss >> res; return res; },                                   \
 { os << t <<std::endl; },                                               \
 { disp(t,os); })

/* Lazy definition of <cast> and <disp> with implementation of
 * <trace> in the cpp.
 */
#define DG_SIGNAL_CAST_DEFINITION_TRACE(TYPE)                            \
 DG_SIGNAL_CAST_FULL_DEFINITION(TYPE,                                    \
 {TYPE res; iss >> res; return res; },                                   \
 { os << t <<std::endl; },                                               \
 ;)

/* Macro to add the define SignalCast in the dg graph. Typical use is:
 *     DG_ADD_CASTER( Matrix,matrix )
 */
#define DG_ADD_CASTER(TYPE,ID)                                            \
  ::dynamicgraph::SignalCastRegisterer sotCastRegisterer_##ID		  \
    (typeid(TYPE),                                                        \
     SignalCast<TYPE>::disp_,                                             \
     SignalCast<TYPE>::cast_,                                             \
     SignalCast<TYPE>::trace_)



#endif  // #ifndef DYNAMIC_GRAPH_SIGNAL_CASTER_HELPER_HH
