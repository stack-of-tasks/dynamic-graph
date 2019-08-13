// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_SIGNAL_PTR_H
#define DYNAMIC_GRAPH_SIGNAL_PTR_H

#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/signal.h>

#include <dynamic-graph/deprecated.hh>

namespace dynamicgraph {
/// \ingroup dgraph
///
/// \brief This is the only type of signal that can be plugged to,
/// using the plug () command.
///
/// In that sense, when plugged into, it acts as a "pointer" to the
/// input signal, hence the name. Operator -> is also overloaded and
/// can be used to access the pointed signal.
template <class T, class Time>
class SignalPtr : public virtual Signal<T, Time> {
 public:
  using SignalBase<Time>::getName;

 protected:
  Signal<T, Time>* signalPtr;
  bool modeNoThrow;
  bool transmitAbstract;
  SignalBase<Time>* abstractTransmitter;
  T* transmitAbstractData;

  inline bool autoref() const { return signalPtr == this; }

 public: /* --- CONSTRUCTORS --- */
  SignalPtr(Signal<T, Time>* ptr, std::string name = "")
      : Signal<T, Time>(name),
        signalPtr(ptr),
        modeNoThrow(false),
        transmitAbstract(false),
        abstractTransmitter(NULL) {}

  virtual ~SignalPtr() { signalPtr = NULL; }

 public:                                           /* --- PLUG-IN OPERATION --- */
  Signal<T, Time>* getPtr();                       // throw
  const Signal<T, Time>* getPtr() const;           // throw
  SignalBase<Time>* getAbstractPtr();              // throw
  const SignalBase<Time>* getAbstractPtr() const;  // throw
  virtual void plug(SignalBase<Time>* ref);

  virtual void unplug() { plug(NULL); }

  virtual bool isPlugged() const { return (NULL != signalPtr); }
  virtual SignalBase<Time>* getPluged() const { return signalPtr; }
  virtual bool isAbstractPluged() const;
  virtual const Time& getTime() const;

  /* Equivalent operator-like definitions. */
  inline Signal<T, Time>* operator->() { return getPtr(); }
  inline const Signal<T, Time>* operator->() const { return getPtr(); }
  inline Signal<T, Time>& operator*() { return *getPtr(); }
  inline const Signal<T, Time>& operator*() const { return *getPtr(); }
  inline operator bool() const { return isPlugged(); }

 public: /* --- INHERITANCE --- */
  virtual bool needUpdate(const Time& t) const;
  virtual std::ostream& writeGraph(std::ostream& os) const;
  virtual std::ostream& display(std::ostream& os) const;

  /* For compatibility, .access () is equivalent to ->access (). For explicit
   * pointer dereference :
   * Prefere -> () to ()
   */
  virtual const T& operator()(const Time& t);
  /* Similarly, Prefere ->access to .access
   */
  virtual const T& access(const Time& t);
  virtual const T& accessCopy() const;

  inline void setConstantDefault(const T& t) {
    Signal<T, Time>::setConstant(t);
    modeNoThrow = true;
  }
  virtual inline void setConstantDefault() { setConstantDefault(accessCopy()); }
  inline void unsetConstantDefault() { modeNoThrow = false; }

  virtual void checkCompatibility();

 public: /* --- INHERITANCE --- */
  /* SignalPtr could be used as a classical signal, through the normal
   * setting functions. The behavior is to plugged the signalPtr on
   * the classical mother Signal layer of the object.
   */
  virtual void setConstant(const T& t) {
    plug(this);
    Signal<T, Time>::setConstant(t);
  }
  virtual void setReference(const T* t, typename Signal<T, Time>::Mutex* m = NULL) {
    plug(this);
    Signal<T, Time>::setReference(t, m);
  }
  virtual void setFunction(boost::function2<T&, T&, Time> t, typename Signal<T, Time>::Mutex* m = NULL) {
    plug(this);
    Signal<T, Time>::setFunction(t, m);
  }

  /*     template< class Provider > */
  /* 	void setFunction( T& (Provider::*fun)(Time,T&),Provider& obj, */
  /* 		  boost::try_mutex *mutexref=NULL ) */
  /* 	{ plug(this); Signal<T,Time>::setFunction(fun,obj,mutexref); } */

  virtual inline Signal<T, Time>& operator=(const T& t) {
    setConstant(t);
    return *this;
  }

  virtual std::ostream& displayDependencies(std::ostream& os, const int depth = -1, std::string space = "",
                                            std::string next1 = "", std::string next2 = "") const;

 protected:  // Interdiction of the rest of the heritage
  using Signal<T, Time>::addDependency;
  virtual void addDependency() {}
  using Signal<T, Time>::removeDependency;
  virtual void removeDependency() {}
  virtual void clearDependencies() {}
};

}  // end of namespace dynamicgraph

#include <dynamic-graph/signal-ptr.t.cpp>
#endif  //! DYNAMIC_GRAPH_SIGNAL_PTR_H
