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

#ifndef __dg_command_direct_getter_h__
#define __dg_command_direct_getter_h__

/* Define a getter command directly on the attribute (no need to pass by
 * an explicit function). A typical use is given here:
 *     addCommand("getSize",
 *                makeDirectGetter(*this,&_dimension,
 *                                 docDirectGetter("dimension","int")));
 *
 */

#include "dynamic-graph/command.h"
#include <boost/assign/list_of.hpp>

/* --- GETTER --------------------------------------------------------- */
namespace dynamicgraph {
  namespace command {

    template <class E, typename T>
      class DirectGetter
      : public Command
    {
    public:
      /// Pointer to method that sets parameter of type T
      typedef T (E::*GetterMethod) () const;

      /// Constructor
    DirectGetter(E& entity,T* ptr,
		 const std::string& docString)
      : Command(entity, std::vector<Value::Type>(), docString),
	T_ptr(ptr) {}

    protected:
      virtual Value doExecute() { return Value(*T_ptr); }
    private:
      T* T_ptr;
    };

    template <class E, typename T>
      DirectGetter<E,T>*
      makeDirectGetter( E& entity,T* ptr,
			const std::string& docString)
      { return  new DirectGetter<E,T>(entity,ptr,docString); }

    inline std::string docDirectGetter( const std::string& name,
					const std::string& type )
      {
	return std::string("\nGet the ")+name+".\n\nNo input.\nReturn an "+type+".\n\n";
      }

  } // namespace command
} // namespace dynamicgraph


#endif // __dg_command_direct_getter_h__
