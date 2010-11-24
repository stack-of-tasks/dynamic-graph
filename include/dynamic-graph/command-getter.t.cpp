//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
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

#ifndef DYNAMIC_GRAPH_COMMAND_GETTER_T_CPP
#define DYNAMIC_GRAPH_COMMAND_GETTER_T_CPP

#include <sstream>

namespace dynamicgraph {
  class Entity;
  namespace command {

    template <class E, typename T>
    Getter<E, T>::Getter(E& entity, GetterMethod getterMethod,
			 const std::string& docstring) :
      Command(entity, std::vector<Value::Type>(), docstring),
      getterMethod_(getterMethod)
    {
    }

    template <class E, typename T>
    Value Getter<E, T>::doExecute()
    {
      E& entity = static_cast<E&>(owner());
      T value = (entity.*getterMethod_)();
      return Value(value);
    }
  } // namespace command
} // namespace dynamicgraph

#endif // DYNAMIC_GRAPH_COMMAND_GETTER_T_CPP
