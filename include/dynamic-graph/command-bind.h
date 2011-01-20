//
// Copyright 2010 CNRS
//
// Author: Nicolas Mansard
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
// dynamic-graph is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.  You should
// have received a copy of the GNU Lesser General Public License along
// with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __dg_command_bind_h__
#define __dg_command_bind_h__

/* Create a command from a bind directly. Examples are:

 *    addCommand("myProcVoid",
 *		 makeCommandVoid0(*this,&ClassName::functionNoArg,
 *				  docCommandVoid0("Simple line doc here.")));

 *    addCommand("myProcOneString",
 *		 makeCommandVoid1(*this,&EntityName::functionOneStringArg,
 *				  docCommandVoid1("Simple line doc here.",
 *						  "string")));
 *
 */

#include "dynamic-graph/command.h"
#include <boost/assign/list_of.hpp>

/* --- FUNCTION 0 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
  namespace command {

#define COMMAND_EMPTY_ARG std::vector<Value::Type>()

    template <class E >
      struct CommandVoid0
      : public Command
    {
    CommandVoid0(E& entity, boost::function<void(void)> function,
		 const std::string& docString)
      :Command(entity, COMMAND_EMPTY_ARG, docString)
	,fptr(function)
      {}

    protected:
      virtual Value doExecute()
      {
	assert( getParameterValues().size() == 0 );
	fptr();
	return Value(); // void
      }
    private:
      boost::function<void(void)> fptr;
    };


    template <class E >
      CommandVoid0<E>* makeCommandVoid0(E& entity, boost::function<void(void)> function ,
    					const std::string& docString)
      {
    	return new CommandVoid0<E>( entity,function,docString );
      }

    template <class E >
      CommandVoid0<E>* makeCommandVoid0(E& entity, boost::function<void(E*)> function ,
    					const std::string& docString)
      {
    	return new CommandVoid0<E>( entity,
    				    boost::bind(function,&entity),docString );
      }

    template <class E >
      CommandVoid0<E>* makeCommandVoid0(E& entity, void (E::*function) (void),
    					const std::string& docString)
      {
    	return new CommandVoid0<E>( entity,
    				    boost::bind(function,&entity),docString );
      }

    std::string docCommandVoid0( const std::string& doc )
      {
	return 	std::string("\n")+doc +"\n\nNo input.\nVoid return.\n\n";
      }

  } // namespace command
} // namespace dynamicgraph

/* --- FUNCTION 1 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
  namespace command {

#define COMMAND_EMPTY_ARG std::vector<Value::Type>()

    template <class E,typename T >
      struct CommandVoid1
      : public Command
    {
      typedef boost::function<void(const T&)> function_t;
      typedef boost::function<void(E*,const T&)> memberFunction_t;
      typedef void (E::*memberFunction_ptr_t) (const T&);

    CommandVoid1(E& entity, function_t function,
		 const std::string& docString)
      :Command(entity, boost::assign::list_of(ValueHelper<T>::TypeID), docString)
	,fptr(function)
      {}

    protected:
      virtual Value doExecute()
      {
	assert( getParameterValues().size() == 1 );
	T val = getParameterValues()[0].value();
	fptr(val);
	return Value(); // void
      }
    private:
      function_t fptr;
    };


    template <class E,typename T >
      CommandVoid1<E,T>*
      makeCommandVoid1(E& entity,
		       typename CommandVoid1<E,T>::function_t function ,
		       const std::string& docString)
      {
    	return new CommandVoid1<E,T>( entity,function,docString );
      }

    template <class E,typename T >
      CommandVoid1<E,T>*
      makeCommandVoid1(E& entity,
		       typename CommandVoid1<E,T>::memberFunction_t function ,
		       const std::string& docString)
      {
    	return new CommandVoid1<E,T>( entity,
				      boost::bind(function,&entity,_1),docString );
      }

    template <class E,typename T >
      CommandVoid1<E,T>*
      makeCommandVoid1(E& entity,
		       void (E::*function) (const T&),
		       const std::string& docString)
      {
	return new CommandVoid1<E,T>( entity,
				      boost::bind(function,&entity,_1),docString );
    	return NULL;
      }

    std::string docCommandVoid1( const std::string& doc, const std::string& type )
      {
	return 	std::string("\n")+doc +"\n\nInput:\n - A "+type+".\nVoid return.\n\n";
      }

  } // namespace command
} // namespace dynamicgraph

#endif // __dg_command_bind_h__
