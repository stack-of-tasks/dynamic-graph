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

#ifndef DYNAMIC_GRAPH_VALUE_H
#define DYNAMIC_GRAPH_VALUE_H

#include <iostream>
#include <string>
#include <cassert>
#include <typeinfo>
#include "dynamic-graph/dynamic-graph-api.h"

namespace dynamicgraph {
  namespace command {
    class Value;
    class AnyType {
    public:
      AnyType(const Value& value);
      operator int () const;
      operator double () const;
      operator std::string () const;
    private:
      const Value& value_;
    };

    class DYNAMICGRAPH_EXPORT Value {
    public:
      enum Type {
	NONE,
	INT,
	DOUBLE,
	STRING,
	NB_TYPES
      };
      ~Value();
      Value(const int& value);
      Value(const double& value);
      Value(const std::string& value);
      /// Copy constructor
      Value(const Value& value);
      // Construct an empty value (None)
      Value();
      /// Return the type of the value
      Type type() const;

      /// Return the value as a castable value into the approriate type
      ///
      /// For instance,
      /// \code
      /// Value v1(5.0); // v1 is of type double
      /// Value v2(3);   // v2 is of type int
      /// double x1 = v1.value();
      /// double x2 = v2.value();
      /// \endcode
      /// The first assignment will succeed, while the second one will throw
      /// an exception.
      const AnyType value () const;
      /// Return the name of the type
      static std::string typeName(Type type);

      /// Output in a stream
      friend std::ostream& operator<<(std::ostream& os, const Value& value);
    private:
      friend class AnyType;
      const double doubleValue() const;
      const int intValue() const;
      const std::string stringValue() const;
      Type type_;
      const void* value_;
    };
} // namespace command
} //namespace dynamicgraph

#endif //DYNAMIC_GRAPH_VALUE_H
