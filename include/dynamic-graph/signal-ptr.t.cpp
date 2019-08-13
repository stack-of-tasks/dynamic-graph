// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_SIGNAL_PTR_T_CPP
#define DYNAMIC_GRAPH_SIGNAL_PTR_T_CPP
#include <dynamic-graph/signal-ptr.h>

#undef VP_TEMPLATE_DEBUG_MODE
#define VP_TEMPLATE_DEBUG_MODE 0
#include <dynamic-graph/debug.h>

namespace dynamicgraph {
template <class T, class Time>
bool SignalPtr<T, Time>::isAbstractPluged() const {
  return ((NULL != signalPtr) || (abstractTransmitter));
}

template <class T, class Time>
Signal<T, Time>* SignalPtr<T, Time>::getPtr() {
  dgTDEBUGIN(25);
  if (!isPlugged())
    DG_THROW ExceptionSignal(ExceptionSignal::NOT_INITIALIZED, "In SignalPtr: SIN ptr not set.", " (in signal <%s>)",
                             getName().c_str());
  dgTDEBUGOUT(25);
  return signalPtr;
}

template <class T, class Time>
const Signal<T, Time>* SignalPtr<T, Time>::getPtr() const {
  dgTDEBUGIN(25) << SignalBase<Time>::name << "(" << isPlugged() << ")" << this << "->" << signalPtr << std::endl;
  dgTDEBUGIN(25);
  if (!isPlugged()) {
    DG_THROW ExceptionSignal(ExceptionSignal::NOT_INITIALIZED, "In SignalPtr: SIN ptr not set.", " (in signal <%s>)",
                             getName().c_str());
  }
  dgTDEBUGOUT(25);
  return signalPtr;
}

template <class T, class Time>
SignalBase<Time>* SignalPtr<T, Time>::getAbstractPtr() {
  if (!isAbstractPluged()) {
    DG_THROW ExceptionSignal(ExceptionSignal::NOT_INITIALIZED, "In SignalPtr: SIN ptr not set.", " (in signal <%s>)",
                             getName().c_str());
  }
  if (NULL != signalPtr)
    return signalPtr;
  else
    return abstractTransmitter;
}

template <class T, class Time>
const SignalBase<Time>* SignalPtr<T, Time>::getAbstractPtr() const {
  if (!isAbstractPluged()) {
    DG_THROW ExceptionSignal(ExceptionSignal::NOT_INITIALIZED, "In SignalPtr: SIN ptr not set.", " (in signal <%s>)",
                             getName().c_str());
  }
  if (NULL != signalPtr)
    return signalPtr;
  else
    return abstractTransmitter;
}

template <class T, class Time>
void SignalPtr<T, Time>::plug(SignalBase<Time>* unknown_ref) {
  dgTDEBUGIN(5);
  if (!unknown_ref) {
    signalPtr = NULL;
    transmitAbstract = false;
    dgTDEBUGOUT(5);
    return;
  }

  dgTDEBUG(5) << "# In  T = " << getName() << " =" << typeid(Signal<T, Time>::Tcopy1).name() << "{ " << std::endl;

  Signal<T, Time>* ref = dynamic_cast<Signal<T, Time>*>(unknown_ref);
  if (NULL == ref) {
    try {
      unknown_ref->checkCompatibility();
    } catch (T* t) {
      dgTDEBUG(25) << "Cast THROW ok." << std::endl;
      Signal<T, Time>::setReference(t);
      transmitAbstract = true;
      abstractTransmitter = unknown_ref;
      transmitAbstractData = t;
    } catch (...) {
      dgTDEBUG(25) << "Fatal error." << std::endl;
      transmitAbstract = false;
      DG_THROW ExceptionSignal(ExceptionSignal::PLUG_IMPOSSIBLE, "Compl. Uncompatible types for plugin.",
                               "(while trying to plug <%s> on <%s>)"
                               " with types <%s> on <%s>.",
                               unknown_ref->getName().c_str(), this->getName().c_str(), typeid(T).name(),
                               typeid(unknown_ref).name());
    }
  } else {
    dgTDEBUG(25) << "Cast ok." << std::endl;
    transmitAbstract = false;
    signalPtr = ref;
  }
  dgTDEBUGOUT(5);
}

template <class T, class Time>
void SignalPtr<T, Time>::checkCompatibility() {
  if (isPlugged() && (!autoref())) {
    getPtr()->checkCompatibility();
  } else if (isAbstractPluged() && (!autoref())) {
    abstractTransmitter->checkCompatibility();
  } else
    Signal<T, Time>::checkCompatibility();
}

template <class T, class Time>
bool SignalPtr<T, Time>::needUpdate(const Time& t) const {
  if ((isAbstractPluged()) && (!autoref())) {
    return getAbstractPtr()->needUpdate(t);
  } else
    return Signal<T, Time>::needUpdate(t);
}

template <class T, class Time>
const Time& SignalPtr<T, Time>::getTime() const {
  if ((isAbstractPluged()) && (!autoref())) {
    return getAbstractPtr()->getTime();
  }
  return Signal<T, Time>::getTime();
}

template <class T, class Time>
const T& SignalPtr<T, Time>::operator()(const Time& t) {
  return access(t);
}

template <class T, class Time>
const T& SignalPtr<T, Time>::access(const Time& t) {
  dgTDEBUGIN(15);
  if (modeNoThrow && (!isPlugged()) && Signal<T, Time>::copyInit) {
    dgTDEBUGOUT(15);
    return Signal<T, Time>::accessCopy();
  } else if (autoref()) {
    dgTDEBUGOUT(15);
    return Signal<T, Time>::access(t);
  } else if (transmitAbstract) {
    abstractTransmitter->recompute(t);
    dgTDEBUGOUT(15);
    return *transmitAbstractData;
  } else {
    dgTDEBUGOUT(15);
    return getPtr()->access(t);
  }
}
template <class T, class Time>
const T& SignalPtr<T, Time>::accessCopy() const {
  if (modeNoThrow && (!isPlugged()) && Signal<T, Time>::copyInit)
    return Signal<T, Time>::accessCopy();
  else if (autoref())
    return Signal<T, Time>::accessCopy();
  else if (transmitAbstract)
    return *transmitAbstractData;
  else
    return getPtr()->accessCopy();
}
template <class T, class Time>
std::ostream& SignalPtr<T, Time>::writeGraph(std::ostream& os) const {
  std::string LeaderLocalName;
  std::string LeaderNodeName;
  Signal<T, Time>::ExtractNodeAndLocalNames(LeaderLocalName, LeaderNodeName);
  if (isAbstractPluged() && !autoref()) {
    std::string itLocalName, itNodeName;
    getAbstractPtr()->ExtractNodeAndLocalNames(itLocalName, itNodeName);
    os << "\t\"" << itNodeName << "\" -> \"" << LeaderNodeName << "\"" << std::endl
       << "\t [ headlabel = \"" << LeaderLocalName << "\" , taillabel = \"" << itLocalName
       << "\", fontsize=7, fontcolor=red ]" << std::endl;
  }
  return os;
}

template <class T, class Time>
std::ostream& SignalPtr<T, Time>::display(std::ostream& os) const {
  dgTDEBUGIN(25) << SignalBase<Time>::name << this << "||" << isPlugged() << "||" << signalPtr;
  { Signal<T, Time>::display(os); }

  if ((isAbstractPluged()) && (!autoref())) {
    // 	    dgTDEBUG(25) << "Display pointed."<<std::endl;
    // 	    getPtr ()->display(os<<"PTR->");
    os << " -->-- PLUGGED";
  } else {
    if (!isAbstractPluged())
      os << " UNPLUGGED";
    else if (autoref())
      os << " AUTOPLUGGED";
  }

  dgTDEBUGOUT(25);
  return os;
}

template <class T, class Time>
std::ostream& SignalPtr<T, Time>::displayDependencies(std::ostream& os, const int depth, std::string space,
                                                      std::string next1, std::string next2) const {
  dgTDEBUGIN(25);
  if ((isAbstractPluged()) && (!autoref())) {
    getAbstractPtr()->displayDependencies(os, depth, space, next1 + "-- " + SignalBase<Time>::name + " -->", next2);
  } else {
    SignalBase<Time>::displayDependencies(os, depth, space, next1, next2);
  }
  dgTDEBUGOUT(25);
  return os;
}

}  // end of namespace dynamicgraph.

#endif  //! DYNAMIC_GRAPH_SIGNAL_PTR_T_CPP
