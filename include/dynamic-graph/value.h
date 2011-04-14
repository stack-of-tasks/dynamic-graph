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
#include "dynamic-graph/linear-algebra.h"

namespace dynamicgraph {
  namespace command {
    class Value;
    class DYNAMIC_GRAPH_DLLEXPORT EitherType {
    public:
      EitherType(const Value& value);
      ~EitherType();
      operator bool () const;
      operator unsigned () const;
      operator int () const;
      operator float () const;
      operator double () const;
      operator std::string () const;
      operator Vector () const;
      operator Matrix () const;
    private:
      const Value* value_;
    };

    class DYNAMIC_GRAPH_DLLEXPORT Value {
    public:
      enum Type {
	NONE,
	BOOL,
	UNSIGNED,
	INT,
	FLOAT,
	DOUBLE,
	STRING,
	VECTOR,
	MATRIX,
	NB_TYPES
      };
      ~Value();
      explicit Value(const bool& value);
      explicit Value(const unsigned& value);
      explicit Value(const int& value);
      explicit Value(const float& value);
      explicit Value(const double& value);
      explicit Value(const std::string& value);
      explicit Value(const Vector& value);
      explicit Value(const Matrix& value);
      /// Copy constructor
      Value(const Value& value);
      // Construct an empty value (None)
      explicit Value();
      // operator assignement
      Value operator=(const Value& value);
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
      const EitherType value () const;
      /// Return the name of the type
      static std::string typeName(Type type);

      /// Output in a stream
      DYNAMIC_GRAPH_DLLEXPORT friend std::ostream& operator<<(std::ostream& os, const Value& value);
    public:
      DYNAMIC_GRAPH_DLLEXPORT friend class EitherType;
      bool boolValue() const;
      unsigned unsignedValue() const;
      int intValue() const;
      float floatValue() const;
      double doubleValue() const;
      std::string stringValue() const;
      Vector vectorValue() const;
      Matrix matrixValue() const;
      Type type_;
      const void* const value_;
    };

    /* ---- HELPER ---------------------------------------------------------- */
    template< typename T >
      struct ValueHelper
      {
	static const Value::Type TypeID;
      };

  } // namespace command
} //namespace dynamicgraph

#endif //DYNAMIC_GRAPH_VALUE_H
