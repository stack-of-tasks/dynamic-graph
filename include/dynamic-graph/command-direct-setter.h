//
// Copyright 2010 CNRS
//
// Author: Nicolas Mansard
//

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

template <class E, typename T> class DirectSetter : public Command {
public:
  DirectSetter(E &entity, T *ptr, const std::string &docString)
      : Command(entity, boost::assign::list_of(ValueHelper<T>::TypeID),
                docString),
        T_ptr(ptr) {}

protected:
  virtual Value doExecute() {
    const std::vector<Value> &values = getParameterValues();
    T val = values[0].value();
    (*T_ptr) = val;
    return Value(); // void
  }

private:
  T *T_ptr;
};

template <class E, typename T>
DirectSetter<E, T> *makeDirectSetter(E &entity, T *ptr,
                                     const std::string &docString) {
  return new DirectSetter<E, T>(entity, ptr, docString);
}

inline std::string docDirectSetter(const std::string &name,
                                   const std::string &type) {
  return std::string("\nSet the ") + name + ".\n\nInput:\n - a " + type +
         ".\nVoid return.\n\n";
}

} // namespace command
} // namespace dynamicgraph

#endif // __dg_command_direct_setter_h__
