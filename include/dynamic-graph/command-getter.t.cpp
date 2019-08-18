//
// Copyright 2010 CNRS
//
// Author: Florent Lamiraux
//

#ifndef DYNAMIC_GRAPH_COMMAND_GETTER_T_CPP
#define DYNAMIC_GRAPH_COMMAND_GETTER_T_CPP

#include <sstream>

namespace dynamicgraph
{
  class Entity;
  namespace command
  {
    
    template <class E, typename T>
    Getter<E, T>::Getter
    (E& entity, GetterMethod getterMethod,
     const std::string& docstring)
      : Command(entity, std::vector<Value::Type>(), docstring),
        getterMethod_(getterMethod)
    {}
    
    template <class E, typename T>
    Value Getter<E, T>::doExecute()
    {
      E& entity = static_cast<E&>(owner());
      T value = (entity.*getterMethod_)();
      return Value(value);
    }
  }  // namespace command
}  // namespace dynamicgraph

#endif  // DYNAMIC_GRAPH_COMMAND_GETTER_T_CPP
