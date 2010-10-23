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

#ifndef DYNAMIC_GRAPH_COMMAND_SETTER_H
#define DYNAMIC_GRAPH_COMMAND_SETTER_H

#include "dynamic-graph/command.h"

namespace dynamicgraph {
  namespace command {

    ///
    /// Command that calls a parameter setter function
    ///
    template <class E, typename T>
    class Setter : public Command {
    public:
      /// Pointer to method that sets paramter of type T
      typedef void (E::*SetterMethod) (const T&);
      /// Constructor
      Setter(E& entity, SetterMethod);
      
    protected:
      virtual Value doExecute();

    private:
      static const std::vector<Value::Type> typeVector();
      SetterMethod setterMethod_;
    };
  } // namespace command
} // namespace dynamicgraph

#include "dynamic-graph/command-setter.t.cpp"
#endif //DYNAMIC_GRAPH_COMMAND_SETTER_H

