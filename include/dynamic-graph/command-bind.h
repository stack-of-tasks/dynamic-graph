//
// Copyright 2010 CNRS
//
// Author: Nicolas Mansard
//

#ifndef __dg_command_bind_h__
#define __dg_command_bind_h__

/* Create a command from a bind directly. Examples are:

 *    addCommand("myProcVoid",
 *               makeCommandVoid0(*this,&ClassName::functionNoArg,
 *                                docCommandVoid0("Simple line doc here.")));

 *    addCommand("myProcOneString",
 *               makeCommandVoid1(*this,&EntityName::functionOneStringArg,
 *                                docCommandVoid1("Simple line doc here.",
 *                                                "string")));
 *
 */

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "dynamic-graph/command.h"

/* --- FUNCTION 0 ARGS ----------------------------------------------------- */
namespace dynamicgraph {
namespace command {

template <class E>
struct CommandVoid0 : public Command {
  CommandVoid0(E &entity, boost::function<void(void)> function,
               const std::string &docString)
      : Command(entity, EMPTY_ARG, docString), fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 0);
    fptr();
    return Value();  // void
  }

 private:
  boost::function<void(void)> fptr;
};

template <class E>
CommandVoid0<E> *makeCommandVoid0(E &entity,
                                  boost::function<void(void)> function,
                                  const std::string &docString) {
  return new CommandVoid0<E>(entity, function, docString);
}

template <class E>
CommandVoid0<E> *makeCommandVoid0(E &entity,
                                  boost::function<void(E *)> function,
                                  const std::string &docString) {
  return new CommandVoid0<E>(entity, boost::bind(function, &entity), docString);
}

template <class E>
CommandVoid0<E> *makeCommandVoid0(E &entity, void (E::*function)(void),
                                  const std::string &docString) {
  return new CommandVoid0<E>(entity, boost::bind(function, &entity), docString);
}

inline std::string docCommandVoid0(const std::string &doc) {
  return std::string("\n") + doc + "\n\nNo input.\nVoid return.\n\n";
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 1 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T>
struct CommandVoid1 : public Command {
  typedef boost::function<void(const T &)> function_t;

  CommandVoid1(E &entity, function_t function, const std::string &docString)
      : Command(entity, boost::assign::list_of(ValueHelper<T>::TypeID),
                docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 1);
    T val = getParameterValues()[0].value();
    fptr(val);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T>
CommandVoid1<E, T> *makeCommandVoid1(
    E &entity, boost::function<void(const T &)> function,
    // typename CommandVoid1<E,T>::function_t function ,
    const std::string &docString) {
  return new CommandVoid1<E, T>(entity, function, docString);
}

template <class E, typename T>
CommandVoid1<E, T> *makeCommandVoid1(
    E &entity,
    // The following syntaxt don't compile when not specializing
    // the template arg... why ???
    boost::function<void(E *, const T &)> function,
    const std::string &docString) {
  return new CommandVoid1<E, T>(entity, boost::bind(function, &entity, _1),
                                docString);
}

template <class E, typename T>
CommandVoid1<E, T> *makeCommandVoid1(E &entity, void (E::*function)(const T &),
                                     const std::string &docString) {
  return new CommandVoid1<E, T>(entity, boost::bind(function, &entity, _1),
                                docString);
  return NULL;
}

inline std::string docCommandVoid1(const std::string &doc,
                                   const std::string &type) {
  return std::string("\n") + doc + "\n\nInput:\n - A " + type +
         ".\nVoid return.\n\n";
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 2 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T1, typename T2>
struct CommandVoid2 : public Command {
  typedef boost::function<void(const T1 &, const T2 &)> function_t;

  CommandVoid2(E &entity, function_t function, const std::string &docString)
      : Command(entity,
                boost::assign::list_of(ValueHelper<T1>::TypeID)(
                    ValueHelper<T2>::TypeID),
                docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 2);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    fptr(val1, val2);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T1, typename T2>
CommandVoid2<E, T1, T2> *makeCommandVoid2(
    E &entity, boost::function<void(const T1 &, const T2 &)> function,
    const std::string &docString) {
  return new CommandVoid2<E, T1, T2>(entity, function, docString);
}

template <class E, typename T1, typename T2>
CommandVoid2<E, T1, T2> *makeCommandVoid2(
    E &entity,
    // The following syntaxt don't compile when not specializing
    // the template arg... why ???
    boost::function<void(E *, const T1 &, const T2 &)> function,
    const std::string &docString) {
  return new CommandVoid2<E, T1, T2>(
      entity, boost::bind(function, &entity, _1, _2), docString);
}

template <class E, typename T1, typename T2>
CommandVoid2<E, T1, T2> *makeCommandVoid2(E &entity,
                                          void (E::*function)(const T1 &,
                                                              const T2 &),
                                          const std::string &docString) {
  return new CommandVoid2<E, T1, T2>(
      entity, boost::bind(function, &entity, _1, _2), docString);
  return NULL;
}

inline std::string docCommandVoid2(const std::string &doc,
                                   const std::string &type1,
                                   const std::string &type2) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" + "Void return.\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 3 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T1, typename T2, typename T3>
struct CommandVoid3 : public Command {
  typedef boost::function<void(const T1 &, const T2 &, const T3 &)> function_t;

  CommandVoid3(E &entity, function_t function, const std::string &docString)
      : Command(entity,
                boost::assign::list_of(ValueHelper<T1>::TypeID)(
                    ValueHelper<T2>::TypeID)(ValueHelper<T3>::TypeID),
                docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 3);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    T3 val3 = getParameterValues()[2].value();
    fptr(val1, val2, val3);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T1, typename T2, typename T3>
CommandVoid3<E, T1, T2, T3> *makeCommandVoid3(
    E &entity, typename CommandVoid3<E, T1, T2, T3>::function_t function,
    const std::string &docString) {
  return new CommandVoid3<E, T1, T2, T3>(entity, function, docString);
}

template <class E, typename T1, typename T2, typename T3>
CommandVoid3<E, T1, T2, T3> *makeCommandVoid3(
    E &entity,
    // The following syntaxt don't compile when not specializing the template
    // arg... why ???
    boost::function<void(E *, const T1 &, const T2 &, const T3 &)> function,
    const std::string &docString) {
  return new CommandVoid3<E, T1, T2, T3>(
      entity, boost::bind(function, &entity, _1, _2, _3), docString);
}

template <class E, typename T1, typename T2, typename T3>
CommandVoid3<E, T1, T2, T3> *makeCommandVoid3(
    E &entity, void (E::*function)(const T1 &, const T2 &, const T3 &),
    const std::string &docString) {
  return new CommandVoid3<E, T1, T2, T3>(
      entity, boost::bind(function, &entity, _1, _2, _3), docString);
  return NULL;
}

inline std::string docCommandVoid3(const std::string &doc,
                                   const std::string &type1,
                                   const std::string &type2,
                                   const std::string &type3) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" + "Input:\n - A " + type3 + ".\n" +
          "Void return.\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 4 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T1, typename T2, typename T3, typename T4>
struct CommandVoid4 : public Command {
  typedef boost::function<void(const T1 &, const T2 &, const T3 &, const T4 &)>
      function_t;
  typedef void (E::*memberFunction_ptr_t)(const T1 &, const T2 &, const T3 &,
                                          const T4 &);

  CommandVoid4(E &entity, function_t function, const std::string &docString)
      : Command(entity,
                boost::assign::list_of(ValueHelper<T1>::TypeID)(
                    ValueHelper<T2>::TypeID)(
                    ValueHelper<T3>::TypeID)(ValueHelper<T4>::TypeID),
                docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 4);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    T3 val3 = getParameterValues()[2].value();
    T4 val4 = getParameterValues()[3].value();
    fptr(val1, val2, val3, val4);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T1, typename T2, typename T3, typename T4>
CommandVoid4<E, T1, T2, T3, T4> *makeCommandVoid4(
    E &entity, typename CommandVoid4<E, T1, T2, T3, T4>::function_t function,
    const std::string &docString) {
  return new CommandVoid4<E, T1, T2, T3, T4>(entity, function, docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4>
CommandVoid4<E, T1, T2, T3, T4> *makeCommandVoid4(
    E &entity,
    boost::function<void(E *, const T1 &, const T2 &, const T3 &, const T4 &)>
        function,
    const std::string &docString) {
  return new CommandVoid4<E, T1, T2, T3, T4>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4), docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4>
CommandVoid4<E, T1, T2, T3, T4> *makeCommandVoid4(
    E &entity,
    void (E::*function)(const T1 &, const T2 &, const T3 &, const T4 &),
    const std::string &docString) {
  return new CommandVoid4<E, T1, T2, T3, T4>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4), docString);
  return NULL;
}

inline std::string docCommandVoid4(const std::string &doc,
                                   const std::string &type1,
                                   const std::string &type2,
                                   const std::string &type3,
                                   const std::string &type4) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" + "Input:\n - A " + type3 + ".\n" +
          "Input:\n - A " + type4 + ".\n" + "Void return.\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 5 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5>
struct CommandVoid5 : public Command {
  typedef boost::function<void(const T1 &, const T2 &, const T3 &, const T4 &,
                               const T5 &)>
      function_t;
  typedef void (E::*memberFunction_ptr_t)(const T1 &, const T2 &, const T3 &,
                                          const T4 &, const T5 &);

  CommandVoid5(E &entity, function_t function, const std::string &docString)
      : Command(
            entity,
            boost::assign::list_of(ValueHelper<T1>::TypeID)(
                ValueHelper<T2>::TypeID)(
                ValueHelper<T3>::TypeID)(ValueHelper<T4>::TypeID)(ValueHelper<
                                                                  T5>::TypeID),
            docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 5);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    T3 val3 = getParameterValues()[2].value();
    T4 val4 = getParameterValues()[3].value();
    T5 val5 = getParameterValues()[4].value();
    fptr(val1, val2, val3, val4, val5);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5>
CommandVoid5<E, T1, T2, T3, T4, T5> *makeCommandVoid5(
    E &entity,
    typename CommandVoid5<E, T1, T2, T3, T4, T5>::function_t function,
    const std::string &docString) {
  return new CommandVoid5<E, T1, T2, T3, T4, T5>(entity, function, docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5>
CommandVoid5<E, T1, T2, T3, T4, T5> *makeCommandVoid5(
    E &entity,
    boost::function<void(E *, const T1 &, const T2 &, const T3 &, const T4 &,
                         const T5 &)>
        function,
    const std::string &docString) {
  return new CommandVoid5<E, T1, T2, T3, T4, T5>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5), docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5>
CommandVoid5<E, T1, T2, T3, T4, T5> *makeCommandVoid5(
    E &entity,
    void (E::*function)(const T1 &, const T2 &, const T3 &, const T4 &,
                        const T5 &),
    const std::string &docString) {
  return new CommandVoid5<E, T1, T2, T3, T4, T5>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5), docString);
  return NULL;
}

inline std::string docCommandVoid5(const std::string &doc,
                                   const std::string &type1,
                                   const std::string &type2,
                                   const std::string &type3,
                                   const std::string &type4,
                                   const std::string &type5) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" + "Input:\n - A " + type3 + ".\n" +
          "Input:\n - A " + type4 + ".\n" + "Input:\n - A " + type5 + ".\n" +
          "Void return.\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 6 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
struct CommandVoid6 : public Command {
  typedef boost::function<void(const T1 &, const T2 &, const T3 &, const T4 &,
                               const T5 &, const T6 &)>
      function_t;
  typedef void (E::*memberFunction_ptr_t)(const T1 &, const T2 &, const T3 &,
                                          const T4 &, const T5 &, const T6 &);

  CommandVoid6(E &entity, function_t function, const std::string &docString)
      : Command(
            entity,
            boost::assign::list_of(ValueHelper<T1>::TypeID)(
                ValueHelper<T2>::TypeID)(
                ValueHelper<T3>::TypeID)(ValueHelper<
                                         T4>::TypeID)(ValueHelper<T5>::
                                                          TypeID)(ValueHelper<
                                                                  T6>::TypeID),
            docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 6);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    T3 val3 = getParameterValues()[2].value();
    T4 val4 = getParameterValues()[3].value();
    T5 val5 = getParameterValues()[4].value();
    T6 val6 = getParameterValues()[5].value();
    fptr(val1, val2, val3, val4, val5, val6);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
CommandVoid6<E, T1, T2, T3, T4, T5, T6> *makeCommandVoid6(
    E &entity,
    typename CommandVoid6<E, T1, T2, T3, T4, T5, T6>::function_t function,
    const std::string &docString) {
  return new CommandVoid6<E, T1, T2, T3, T4, T5, T6>(entity, function,
                                                     docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
CommandVoid6<E, T1, T2, T3, T4, T5, T6> *makeCommandVoid6(
    E &entity,
    boost::function<void(E *, const T1 &, const T2 &, const T3 &, const T4 &,
                         const T5 &, const T6 &)>
        function,
    const std::string &docString) {
  return new CommandVoid6<E, T1, T2, T3, T4, T5, T6>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5, _6),
      docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6>
CommandVoid6<E, T1, T2, T3, T4, T5, T6> *makeCommandVoid6(
    E &entity,
    void (E::*function)(const T1 &, const T2 &, const T3 &, const T4 &,
                        const T5 &, const T6 &),
    const std::string &docString) {
  return new CommandVoid6<E, T1, T2, T3, T4, T5, T6>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5, _6),
      docString);
  return NULL;
}

inline std::string docCommandVoid6(
    const std::string &doc, const std::string &type1, const std::string &type2,
    const std::string &type3, const std::string &type4,
    const std::string &type5, const std::string &type6) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" + "Input:\n - A " + type3 + ".\n" +
          "Input:\n - A " + type4 + ".\n" + "Input:\n - A " + type5 + ".\n" +
          "Input:\n - A " + type6 + ".\n" + "Void return.\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 7 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
struct CommandVoid7 : public Command {
  typedef boost::function<void(const T1 &, const T2 &, const T3 &, const T4 &,
                               const T5 &, const T6 &, const T7 &)>
      function_t;
  typedef void (E::*memberFunction_ptr_t)(const T1 &, const T2 &, const T3 &,
                                          const T4 &, const T5 &, const T6 &,
                                          const T7 &);

  CommandVoid7(E &entity, function_t function, const std::string &docString)
      : Command(
            entity,
            boost::assign::list_of(ValueHelper<T1>::TypeID)(
                ValueHelper<T2>::TypeID)(
                ValueHelper<T3>::
                    TypeID)(ValueHelper<
                            T4>::TypeID)(ValueHelper<T5>::
                                             TypeID)(ValueHelper<T6>::
                                                         TypeID)(ValueHelper<
                                                                 T7>::TypeID),
            docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 7);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    T3 val3 = getParameterValues()[2].value();
    T4 val4 = getParameterValues()[3].value();
    T5 val5 = getParameterValues()[4].value();
    T6 val6 = getParameterValues()[5].value();
    T7 val7 = getParameterValues()[6].value();
    fptr(val1, val2, val3, val4, val5, val6, val7);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
CommandVoid7<E, T1, T2, T3, T4, T5, T6, T7> *makeCommandVoid7(
    E &entity,
    typename CommandVoid7<E, T1, T2, T3, T4, T5, T6, T7>::function_t function,
    const std::string &docString) {
  return new CommandVoid7<E, T1, T2, T3, T4, T5, T6, T7>(entity, function,
                                                         docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
CommandVoid7<E, T1, T2, T3, T4, T5, T6, T7> *makeCommandVoid7(
    E &entity,
    boost::function<void(E *, const T1 &, const T2 &, const T3 &, const T4 &,
                         const T5 &, const T6 &, const T7 &)>
        function,
    const std::string &docString) {
  return new CommandVoid7<E, T1, T2, T3, T4, T5, T6, T7>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5, _6, _7),
      docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7>
CommandVoid7<E, T1, T2, T3, T4, T5, T6, T7> *makeCommandVoid7(
    E &entity,
    void (E::*function)(const T1 &, const T2 &, const T3 &, const T4 &,
                        const T5 &, const T6 &, const T7 &),
    const std::string &docString) {
  return new CommandVoid7<E, T1, T2, T3, T4, T5, T6, T7>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5, _6, _7),
      docString);
  return NULL;
}

inline std::string docCommandVoid7(
    const std::string &doc, const std::string &type1, const std::string &type2,
    const std::string &type3, const std::string &type4,
    const std::string &type5, const std::string &type6,
    const std::string &type7) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" + "Input:\n - A " + type3 + ".\n" +
          "Input:\n - A " + type4 + ".\n" + "Input:\n - A " + type5 + ".\n" +
          "Input:\n - A " + type6 + ".\n" + "Input:\n - A " + type7 + ".\n" +
          "Void return.\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 8 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
struct CommandVoid8 : public Command {
  typedef boost::function<void(const T1 &, const T2 &, const T3 &, const T4 &,
                               const T5 &, const T6 &, const T7 &, const T8 &)>
      function_t;
  typedef void (E::*memberFunction_ptr_t)(const T1 &, const T2 &, const T3 &,
                                          const T4 &, const T5 &, const T6 &,
                                          const T7 &, const T8 &);

  CommandVoid8(E &entity, function_t function, const std::string &docString)
      : Command(
            entity,
            boost::assign::list_of(ValueHelper<T1>::TypeID)(
                ValueHelper<T2>::TypeID)(
                ValueHelper<T3>::
                    TypeID)(ValueHelper<T4>::
                                TypeID)(ValueHelper<T5>::
                                            TypeID)(ValueHelper<T6>::
                                                        TypeID)(ValueHelper<
                                                                T7>::
                                                                    TypeID)(ValueHelper<
                                                                            T8>::
                                                                                TypeID),
            docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 8);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    T3 val3 = getParameterValues()[2].value();
    T4 val4 = getParameterValues()[3].value();
    T5 val5 = getParameterValues()[4].value();
    T6 val6 = getParameterValues()[5].value();
    T7 val7 = getParameterValues()[6].value();
    T8 val8 = getParameterValues()[7].value();
    fptr(val1, val2, val3, val4, val5, val6, val7, val8);
    return Value();  // void
  }

 private:
  function_t fptr;
};

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
CommandVoid8<E, T1, T2, T3, T4, T5, T6, T7, T8> *makeCommandVoid8(
    E &entity,
    typename CommandVoid8<E, T1, T2, T3, T4, T5, T6, T7, T8>::function_t
        function,
    const std::string &docString) {
  return new CommandVoid8<E, T1, T2, T3, T4, T5, T6, T7, T8>(entity, function,
                                                             docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
CommandVoid8<E, T1, T2, T3, T4, T5, T6, T7, T8> *makeCommandVoid8(
    E &entity,
    boost::function<void(E *, const T1 &, const T2 &, const T3 &, const T4 &,
                         const T5 &, const T6 &, const T7 &, const T8 &)>
        function,
    const std::string &docString) {
  return new CommandVoid8<E, T1, T2, T3, T4, T5, T6, T7, T8>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5, _6, _7, _8),
      docString);
}

template <class E, typename T1, typename T2, typename T3, typename T4,
          typename T5, typename T6, typename T7, typename T8>
CommandVoid8<E, T1, T2, T3, T4, T5, T6, T7, T8> *makeCommandVoid8(
    E &entity,
    void (E::*function)(const T1 &, const T2 &, const T3 &, const T4 &,
                        const T5 &, const T6 &, const T7 &, const T8 &),
    const std::string &docString) {
  return new CommandVoid8<E, T1, T2, T3, T4, T5, T6, T7, T8>(
      entity, boost::bind(function, &entity, _1, _2, _3, _4, _5, _6, _7, _8),
      docString);
  return NULL;
}

inline std::string docCommandVoid8(
    const std::string &doc, const std::string &type1, const std::string &type2,
    const std::string &type3, const std::string &type4,
    const std::string &type5, const std::string &type6,
    const std::string &type7, const std::string &type8) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" + "Input:\n - A " + type3 + ".\n" +
          "Input:\n - A " + type4 + ".\n" + "Input:\n - A " + type5 + ".\n" +
          "Input:\n - A " + type6 + ".\n" + "Input:\n - A " + type7 + ".\n" +
          "Input:\n - A " + type8 + ".\n" + "Void return.\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION VERBOSE ----------------------------------------------------- */
/* This bind a function void f( ostream& ) that display some results into
 * a string f( void ) that return some string results. */

namespace dynamicgraph {
namespace command {
template <class E>
struct CommandVerbose : public Command {
  typedef boost::function<void(std::ostream &)> function_t;

  CommandVerbose(E &entity, function_t function, const std::string &docString)
      : Command(entity, EMPTY_ARG, docString), fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 0);
    std::ostringstream oss;
    fptr(oss);
    return Value(oss.str());  // return string
  }

 private:
  function_t fptr;
};

template <class E>
CommandVerbose<E> *makeCommandVerbose(
    E &entity, typename CommandVerbose<E>::function_t function,
    const std::string &docString) {
  return new CommandVerbose<E>(entity, function, docString);
  return NULL;
}

template <class E>
CommandVerbose<E> *makeCommandVerbose(E &entity,
                                      void (E::*function)(std::ostream &),
                                      const std::string &docString) {
  return new CommandVerbose<E>(entity, boost::bind(function, &entity, _1),
                               docString);
  return NULL;
}

inline std::string docCommandVerbose(const std::string &doc) {
  return std::string("\n") + doc + "\n\nNo input.\n Return a string.\n\n";
}
/*************************/
/* Template return types */
/*************************/

template <class E, class ReturnType>
struct CommandReturnType0 : public Command {
  CommandReturnType0(E &entity, boost::function<ReturnType(void)> function,
                     const std::string &docString)
      : Command(entity, EMPTY_ARG, docString), fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 0);
    Value res(fptr());
    return res;
  }

 private:
  boost::function<ReturnType(void)> fptr;
};

template <class E, class ReturnType>
CommandReturnType0<E, ReturnType> *makeCommandReturnType0(
    E &entity, boost::function<ReturnType(void)> function,
    const std::string &docString) {
  return new CommandReturnType0<E, ReturnType>(entity, function, docString);
}

template <class E, class ReturnType>
CommandReturnType0<E, ReturnType> *makeCommandReturnType0(
    E &entity, boost::function<ReturnType(E *)> function,
    const std::string &docString) {
  return new CommandReturnType0<E, ReturnType>(
      entity, boost::bind(function, &entity), docString);
}

template <class E, class ReturnType>
CommandReturnType0<E, ReturnType> *makeCommandReturnType0(
    E &entity, ReturnType (E::*function)(void), const std::string &docString) {
  return new CommandReturnType0<E, ReturnType>(
      entity, boost::bind(function, &entity), docString);
}

template <typename ReturnType>
inline std::string docCommandReturnType0(
    const std::string &doc, const std::string & /* return_type */) {
  return std::string("\n") + doc + "\n\nNo input.\n" +
         typeid(ReturnType).name() + " return.\n\n";
}

}  // namespace command
}  // namespace dynamicgraph

/* --- FUNCTION 1 ARGS ------------------------------------------------------ */
namespace dynamicgraph {
namespace command {

template <class E, typename ReturnType, typename T>
struct CommandReturnType1 : public Command {
  typedef boost::function<ReturnType(const T &)> function_t;

  CommandReturnType1(E &entity, function_t function,
                     const std::string &docString)
      : Command(entity, boost::assign::list_of(ValueHelper<T>::TypeID),
                docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 1);
    T val = getParameterValues()[0].value();
    Value res(fptr(val));
    return res;
  }

 private:
  function_t fptr;
};

template <class E, typename ReturnType, typename T>
CommandReturnType1<E, ReturnType, T> *makeCommandReturnType1(
    E &entity, boost::function<ReturnType(const T &)> function,
    const std::string &docString) {
  return new CommandReturnType1<E, ReturnType, T>(entity, function, docString);
}

template <class E, typename ReturnType, typename T>
CommandReturnType1<E, ReturnType, T> *makeCommandReturnType1(
    E &entity,
    // The following syntaxt don't compile when not
    // specializing the template arg... why ???
    boost::function<ReturnType(E *, const T &)> function,
    const std::string &docString) {
  return new CommandReturnType1<E, ReturnType, T>(
      entity, boost::bind(function, &entity, _1), docString);
}

template <class E, typename ReturnType, typename T>
CommandReturnType1<E, ReturnType, T> *makeCommandReturnType1(
    E &entity, ReturnType (E::*function)(const T &),
    const std::string &docString) {
  return new CommandReturnType1<E, ReturnType, T>(
      entity, boost::bind(function, &entity, _1), docString);
  return NULL;
}

template <typename ReturnType>
inline std::string docCommandReturnType1(const std::string &doc,
                                         const std::string &type) {
  return std::string("\n") + doc + "\n\nInput:\n - A " + type + ".\n" +
         typeid(ReturnType).name() + "return.\n\n";
}

}  // namespace command
}  // namespace dynamicgraph

/*********** FUNCTION 2 Arguments ************************/
namespace dynamicgraph {
namespace command {

template <class E, typename ReturnType, typename T1, typename T2>
struct CommandReturnType2 : public Command {
  typedef boost::function<ReturnType(const T1 &, const T2 &)> function_t;

  CommandReturnType2(E &entity, function_t function,
                     const std::string &docString)
      : Command(entity,
                boost::assign::list_of(ValueHelper<T1>::TypeID)(
                    ValueHelper<T2>::TypeID),
                docString),
        fptr(function) {}

 protected:
  virtual Value doExecute() {
    assert(getParameterValues().size() == 2);
    T1 val1 = getParameterValues()[0].value();
    T2 val2 = getParameterValues()[1].value();
    Value res(fptr(val1, val2));
    return res;
  }

 private:
  function_t fptr;
};

template <class E, typename ReturnType, typename T1, typename T2>
CommandReturnType2<E, ReturnType, T1, T2> *makeCommandReturnType2(
    E &entity, boost::function<ReturnType(const T1 &, const T2 &)> function,
    const std::string &docString) {
  return new CommandReturnType2<E, ReturnType, T1, T2>(entity, function,
                                                       docString);
}

template <class E, typename ReturnType, typename T1, typename T2>
CommandReturnType2<E, ReturnType, T1, T2> *makeCommandReturnType2(
    E &entity,
    // The following syntaxt don't compile when not specializing the template
    // arg... why ???
    boost::function<ReturnType(E *, const T1 &, const T2 &)> function,
    const std::string &docString) {
  return new CommandReturnType2<E, ReturnType, T1, T2>(
      entity, boost::bind(function, &entity, _1, _2), docString);
}

template <class E, typename ReturnType, typename T1, typename T2>
CommandReturnType2<E, ReturnType, T1, T2> *makeCommandReturnType2(
    E &entity, ReturnType (E::*function)(const T1 &, const T2 &),
    const std::string &docString) {
  return new CommandReturnType2<E, ReturnType, T1, T2>(
      entity, boost::bind(function, &entity, _1, _2), docString);
  return NULL;
}

template <typename ReturnType>
inline std::string docCommandReturnType2(const std::string &doc,
                                         const std::string &type1,
                                         const std::string &type2) {
  return (std::string("\n") + doc + "\n\n" + "Input:\n - A " + type1 + ".\n" +
          "Input:\n - A " + type2 + ".\n" +
          "ReturnType:\n - Returns:" + typeid(ReturnType).name() + +".\n\n");
}

}  // namespace command
}  // namespace dynamicgraph

#endif  // __dg_command_bind_h__
