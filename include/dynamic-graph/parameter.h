#ifndef DYNAMIC_GRAPH_PARAMETER_H
#define DYNAMIC_GRAPH_PARAMETER_H

#include <boost/static_assert.hpp>
#include "dynamicgraph/exception-abstract.h"

namespace dynamicgraph {
  namespace command {
    class Parameter {
    public:
      Parameter(Value::Type valueType) : valueType_(valueType) {}
      /// Return value type of parameter
      Value::Type valueType() { return valueType_;}
    private:
      Value::Type valueType_;
    }; // class Parameter
  } // namespace command
} //namespace dynamicgraph
