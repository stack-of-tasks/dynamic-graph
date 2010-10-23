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

#ifndef DYNAMIC_GRAPH_COMMAND_SETTER_T_CPP
#define DYNAMIC_GRAPH_COMMAND_SETTER_T_CPP

#include <sstream>

namespace dynamicgraph {
  class Entity;
  namespace command {

    template <class E, typename T>
    const std::vector<Value::Type>  Setter<E, T>::typeVector()
    {
      std::vector<Value::Type> result;
      if (typeid(T) == typeid(int)) {
	result.push_back(Value::INT);
      } else if (typeid(T) == typeid(double)) {
	result.push_back(Value::DOUBLE);
      } else if (typeid(T) == typeid(std::string)) {
	result.push_back(Value::STRING);
      } else {
	std::stringstream ss;
	ss << "Type " << typeid(T).name() << " not supported.";
	throw ExceptionAbstract(ExceptionAbstract::TOOLS,
				ss.str());
      }
      return result;
    }

    template <class E, typename T>
    Setter<E, T>::Setter(E& entity, SetterMethod setterMethod) :
      Command(entity, typeVector()),
      setterMethod_(setterMethod)
    {
    }

    template <class E, typename T>
    Value Setter<E, T>::doExecute()
    {
      const std::vector<Value>& values = getParameterValues();
      // Get parameter
      T value = values[0].value();
      E& entity = static_cast<E&>(owner());
      (entity.*setterMethod_)(value);
      return Value();
    }
  } // namespace command
} // namespace dynamicgraph

#endif // DYNAMIC_GRAPH_COMMAND_SETTER_T_CPP
