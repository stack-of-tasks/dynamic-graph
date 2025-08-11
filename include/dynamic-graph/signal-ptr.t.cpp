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
Signal<T, Time> *SignalPtr<T, Time>::getPtr() {
  dgTDEBUGIN(25);
  if (!isPlugged()) DG_THROW
  ExceptionSignal(ExceptionSignal::NOT_INITIALIZED,
                  "In SignalPtr: SIN ptr not set.", " (in signal <%s>)",
                  getName().c_str());
  dgTDEBUGOUT(25);
  return signalPtr;
}

template <class T, class Time>
const Signal<T, Time> *SignalPtr<T, Time>::getPtr() const {
  dgTDEBUGIN(25) << SignalBase<Time>::name << "(" << isPlugged() << ")" << this
                 << "->" << signalPtr << std::endl;
  dgTDEBUGIN(25);
  if (!isPlugged()) {
    DG_THROW ExceptionSignal(ExceptionSignal::NOT_INITIALIZED,
                             "In SignalPtr: SIN ptr not set.",
                             " (in signal <%s>)", getName().c_str());
  }
  dgTDEBUGOUT(25);
  return signalPtr;
}

template <class T, class Time>
void SignalPtr<T, Time>::plug(SignalBase<Time> *unknown_ref) {
  dgTDEBUGIN(5);
  if (!unknown_ref) {
    signalPtr = NULL;
    dgTDEBUGOUT(5);
    return;
  }

  dgTDEBUG(5) << "# In  T = " << getName() << " ="
              << typeid(Signal<T, Time>::Tcopy1).name() << "{ " << std::endl;

  Signal<T, Time> *ref = dynamic_cast<Signal<T, Time> *>(unknown_ref);
  if (NULL != ref) {
    dgTDEBUG(25) << "Cast ok." << std::endl;
    signalPtr = ref;
  }
  dgTDEBUGOUT(5);
}

template <class T, class Time>
bool SignalPtr<T, Time>::needUpdate(const Time &t) const {
  if ((isPlugged()) && (!autoref())) {
    return getPtr()->needUpdate(t);
  } else
    return Signal<T, Time>::needUpdate(t);
}

template <class T, class Time>
const Time &SignalPtr<T, Time>::getTime() const {
  if ((isPlugged()) && (!autoref())) {
    return getPtr()->getTime();
  }
  return Signal<T, Time>::getTime();
}

template <class T, class Time>
const T &SignalPtr<T, Time>::operator()(const Time &t) {
  return access(t);
}

template <class T, class Time>
const T &SignalPtr<T, Time>::access(const Time &t) {
  dgTDEBUGIN(15);
  if (modeNoThrow && (!isPlugged()) && Signal<T, Time>::copyInit) {
    dgTDEBUGOUT(15);
    return Signal<T, Time>::accessCopy();
  } else if (autoref()) {
    dgTDEBUGOUT(15);
    return Signal<T, Time>::access(t);
  } else {
    dgTDEBUGOUT(15);
    return getPtr()->access(t);
  }
}

template <class T, class Time>
const T &SignalPtr<T, Time>::accessCopy() const {
  if (modeNoThrow && (!isPlugged()) && Signal<T, Time>::copyInit)
    return Signal<T, Time>::accessCopy();
  else if (autoref())
    return Signal<T, Time>::accessCopy();
  else
    return getPtr()->accessCopy();
}
template <class T, class Time>
std::ostream &SignalPtr<T, Time>::writeGraph(std::ostream &os) const {
  std::string LeaderLocalName;
  std::string LeaderNodeName;
  Signal<T, Time>::ExtractNodeAndLocalNames(LeaderLocalName, LeaderNodeName);
  if (isPlugged() && !autoref()) {
    std::string itLocalName, itNodeName;
    getPtr()->ExtractNodeAndLocalNames(itLocalName, itNodeName);
    os << "\t\"" << itNodeName << "\" -> \"" << LeaderNodeName << "\""
       << std::endl
       << "\t [ headlabel = \"" << LeaderLocalName << "\" , taillabel = \""
       << itLocalName << "\", fontsize=7, fontcolor=red ]" << std::endl;
  }
  return os;
}

template <class T, class Time>
std::ostream &SignalPtr<T, Time>::display(std::ostream &os) const {
  dgTDEBUGIN(25) << SignalBase<Time>::name << this << "||" << isPlugged()
                 << "||" << signalPtr;
  {
    Signal<T, Time>::display(os);
  }

  if ((isPlugged()) && (!autoref())) {
    os << " -->-- PLUGGED";
  } else {
    if (!isPlugged())
      os << " UNPLUGGED";
    else if (autoref())
      os << " AUTOPLUGGED";
  }

  dgTDEBUGOUT(25);
  return os;
}

template <class T, class Time>
std::ostream &SignalPtr<T, Time>::displayDependencies(std::ostream &os,
                                                      const int depth,
                                                      std::string space,
                                                      std::string next1,
                                                      std::string next2) const {
  dgTDEBUGIN(25);
  if ((isPlugged()) && (!autoref())) {
    getPtr()->displayDependencies(
        os, depth, space, next1 + "-- " + SignalBase<Time>::name + " -->",
        next2);
  } else {
    SignalBase<Time>::displayDependencies(os, depth, space, next1, next2);
  }
  dgTDEBUGOUT(25);
  return os;
}

}  // end of namespace dynamicgraph.

#endif  //! DYNAMIC_GRAPH_SIGNAL_PTR_T_CPP
