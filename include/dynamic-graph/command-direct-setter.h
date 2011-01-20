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

#ifndef __dg_command_direct_setter_h__
#define __dg_command_direct_setter_h__

/* Define a setter command directly on the attribute (no need to pass by
 * an explicit function). A typical use is given here:
 *     addCommand("setSize",
 *                makeDirectSetter(*this,&_dimension,
 *                                 docDirectSetter("dimension","int")));
 *
 */

#include "dynamic-graph/command.h"
#include <boost/assign/list_of.hpp>

/* --- SETTER --------------------------------------------------------- */
namespace dynamicgraph {
  namespace command {

   template <class E, typename T>
      class DirectSetter
      : public Command
    {
    public:
    DirectSetter(E& entity,T* ptr,const std::string& docString)
      :Command(entity, boost::assign::list_of(ValueHelper<T>::TypeID), docString)
	,T_ptr(ptr)
      {}

    protected:
      virtual Value doExecute()
      {
	const std::vector<Value>& values = getParameterValues();
	T val = values[0].value();
	(*T_ptr) = val;
	return Value(); // void
      }
    private:
      T* T_ptr;
    };

    template <class E, typename T>
      DirectSetter<E,T>*
      makeDirectSetter( E& entity,T* ptr,
			const std::string& docString)
      { return  new DirectSetter<E,T>(entity,ptr,docString); }

    std::string docDirectSetter( const std::string& name,
				 const std::string& type )
      {
	return std::string("\nSet the ")+name+".\n\nInput:\n - a "
	  +type+".\nVoid return.\n\n";
      }

  } // namespace command
} // namespace dynamicgraph


#endif // __dg_command_direct_setter_h__
