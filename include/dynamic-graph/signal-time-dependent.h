// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_SIGNAL_TIME_DEPENDENT_H
#define DYNAMIC_GRAPH_SIGNAL_TIME_DEPENDENT_H
#include <dynamic-graph/signal.h>
#include <dynamic-graph/time-dependency.h>

namespace dynamicgraph {
/*!  \brief A type of signal that enforces a time dependency between other
  signals,
  making sure its inputs are up to date on access, using a incrementing time
  tick as reference.

  It works this way. For a given SignalTimeDependent S,
  - the user manually adds dependent signals through the use of the
  SignalTimeDependent::addDependency function.
  - On access (calling the signal S SignalTimeDependent::operator()(const Time&)
  or SignalTimeDependent::access(const Time&) function), if the dependent
  signals are not up-to-date, i.e. if their [last update] time is less than the
  current time, their value will be SignalTimeDependent::access ()'ed to bring
  them up-to-date.

  Thus, the value of dependent signals can be accessed \b quickly and
  \b repeatedly through the Signal::accessCopy () function.

  An example:
  \code
  class MyEntity : public Entity {
  public:
  // Some signal dependencies
  SignalPtr<T,int> dep1, dep2;
  SignalTimeDependent<T,int> signal;

  MyEntity (const std::string& name)
    : Entity (name)
    , signal (
        // Set the function that computes the signal value
        boost::bind (&Entity::computeSignal, this, boost::placeholders::_1,
  boost::placeholders::_2),
        // Declare the dependencies
        dep1 << dep2,
        "signalname")
  {}

   T& computeSignal (T& res, int time)
   {
     // The accesses below update the signal if necessary.
     dep1(time);
     dep1.access(time);
     dep1.recompute(time);
     // If dep1 and dep2 are already up-to-date, for a faster access, use
     dep1.accessCopy();
     dep2.accessCopy();

     // Compute res
     return res;
   }
  \endcode
*/
template <class T, class Time>
class SignalTimeDependent : public virtual Signal<T, Time>,
                            public TimeDependency<Time> {
  // TimeDependency<Time> timeDependency;

public:
  SignalTimeDependent(std::string name = "");
  SignalTimeDependent(const SignalArray_const<Time> &arr,
                      std::string name = "");
  SignalTimeDependent(boost::function2<T &, T &, Time> t,
                      const SignalArray_const<Time> &sig,
                      std::string name = "");

  virtual ~SignalTimeDependent() {}

  inline const T &operator()(const Time &t1) { return access(t1); }
  const T &access(const Time &t1);

  virtual void addDependency(const SignalBase<Time> &signal);
  virtual void removeDependency(const SignalBase<Time> &signal);
  virtual void clearDependencies();

  std::ostream &writeGraph(std::ostream &os) const { return os; }

  std::ostream &displayDependencies(std::ostream &os, const int depth = -1,
                                    std::string space = "",
                                    std::string next1 = "",
                                    std::string next2 = "") const {
    return TimeDependency<Time>::displayDependencies(os, depth, space, next1,
                                                     next2);
  }

  virtual bool needUpdate(const Time &t) const;
  virtual void setPeriodTime(const Time &p);
  virtual Time getPeriodTime() const;
};

/* -------------------------------------------- */

template <class T, class Time>
SignalTimeDependent<T, Time>::SignalTimeDependent(std::string name)
    : Signal<T, Time>(name), TimeDependency<Time>(this) {}

template <class T, class Time>
SignalTimeDependent<T, Time>::SignalTimeDependent(
    const SignalArray_const<Time> &arr, std::string name)
    : Signal<T, Time>(name), TimeDependency<Time>(this, arr) {}

template <class T, class Time>
SignalTimeDependent<T, Time>::SignalTimeDependent(
    boost::function2<T &, T &, Time> t, const SignalArray_const<Time> &sig,
    std::string name)
    : Signal<T, Time>(name), TimeDependency<Time>(this, sig) {
  this->setFunction(t);
}

template <class T, class Time>
const T &SignalTimeDependent<T, Time>::access(const Time &t1) {
  const bool up = TimeDependency<Time>::needUpdate(t1);
  // SignalBase<Time>::setReady(false);

  /*       std::cout << "Time before: "<< signalTime << " -- "   */
  /*            << t1<< "  -> Up: "<<up <<std::endl ;   */
  if (up) {
    TimeDependency<Time>::lastAskForUpdate = false;
    const T &Tres = Signal<T, Time>::access(t1);
    SignalBase<Time>::setReady(false);
    return Tres;
  } else {
    return Signal<T, Time>::accessCopy();
  }
}

template <class T, class Time>
void SignalTimeDependent<T, Time>::addDependency(
    const SignalBase<Time> &signal) {
  TimeDependency<Time>::addDependency(signal);
}

template <class T, class Time>
void SignalTimeDependent<T, Time>::removeDependency(
    const SignalBase<Time> &signal) {
  TimeDependency<Time>::removeDependency(signal);
}

template <class T, class Time>
void SignalTimeDependent<T, Time>::clearDependencies() {
  TimeDependency<Time>::clearDependency();
}

template <class T, class Time>
bool SignalTimeDependent<T, Time>::needUpdate(const Time &t) const {
  return TimeDependency<Time>::needUpdate(t);
}

template <class T, class Time>
void SignalTimeDependent<T, Time>::setPeriodTime(const Time &p) {
  TimeDependency<Time>::setPeriodTime(p);
}
template <class T, class Time>
Time SignalTimeDependent<T, Time>::getPeriodTime() const {
  return TimeDependency<Time>::getPeriodTime();
}

} // end of namespace dynamicgraph

#endif //! DYNAMIC_GRAPH_SIGNAL_TIME_DEPENDENT_H
