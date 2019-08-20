// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_TIME_DEPENDENCY_T_CPP
#define DYNAMIC_GRAPH_TIME_DEPENDENCY_T_CPP
#include <dynamic-graph/time-dependency.h>

#undef VP_TEMPLATE_DEBUG_MODE
#define VP_TEMPLATE_DEBUG_MODE 0
#include <dynamic-graph/debug.h>

#define __TIME_DEPENDENCY_INIT(sig, dep)                                       \
  leader(*sig), dependencies(), updateFromAllChildren(ALL_READY_DEFAULT),      \
      dependencyType(dep), periodTime(PERIOD_TIME_DEFAULT)

namespace dynamicgraph {
template <class Time>
TimeDependency<Time>::TimeDependency(SignalBase<Time> *sig,
                                     const DependencyType dep)
    : __TIME_DEPENDENCY_INIT(sig, dep) {}

template <class Time>
TimeDependency<Time>::TimeDependency(SignalBase<Time> *sig,
                                     const SignalArray_const<Time> &ar,
                                     const DependencyType dep)
    : __TIME_DEPENDENCY_INIT(sig, dep) {
  for (unsigned int i = 0; i < ar.getSize(); ++i) {
    addDependency(ar[i]);
  }

  return;
}

/* ------------------------------------------------------------------------ */
template <class Time>
void TimeDependency<Time>::addDependency(const SignalBase<Time> &sig) {
  dependencies.push_front(&sig);
}

template <class Time>
void TimeDependency<Time>::removeDependency(const SignalBase<Time> &sig) {
  dependencies.remove(&sig);
}

template <class Time> void TimeDependency<Time>::clearDependency() {
  dependencies.clear();
}

template <class Time>
bool TimeDependency<Time>::needUpdate(const Time &t1) const {
  dgTDEBUG(15) << "# In {" << leader.getName() << " : (" << leader.getReady()
               << ") " << t1 << " >? " << leader.getTime() << std::endl;

  if (leader.getReady()) {
    dgTDEBUGOUT(15);
    return true;
  }
  if (lastAskForUpdate) {
    dgTDEBUGOUT(15);
    return true;
  }

  switch (dependencyType) {
  case ALWAYS_READY: {
    dgTDEBUGOUT(15);
    return true;
  }
  case BOOL_DEPENDENT:
    break;
  case TIME_DEPENDENT: {
    if (t1 < leader.getTime() + periodTime) {
      dgTDEBUGOUT(15);
      return false;
    }
    break;
  }
  };

  bool res = updateFromAllChildren;
  const typename Dependencies::const_iterator itend = dependencies.end();
  for (typename Dependencies::const_iterator it = dependencies.begin();
       it != itend; ++it) {
    const SignalBase<Time> &sig = **it;
    dgTDEBUG(15) << "Ask update for " << sig << std::endl;

    if ((sig.getTime() > leader.getTime()) || (sig.needUpdate(t1))) {
      if (updateFromAllChildren)
        continue;
      else {
        res = true;
        break;
      }
    } else {
      if (updateFromAllChildren) {
        res = false;
        break;
      } else
        continue;
    }
  }
  lastAskForUpdate = res;

  dgTDEBUGOUT(15);
  return res;
}

template <class Time>
std::ostream &TimeDependency<Time>::writeGraph(std::ostream &os) const {
  std::string LeaderLocalName;
  std::string LeaderNodeName;
  leader.ExtractNodeAndLocalNames(LeaderLocalName, LeaderNodeName);
  if (dependencies.size() != 0) {
    const typename Dependencies::const_iterator itend = dependencies.end();
    for (typename Dependencies::const_iterator it = dependencies.begin();
         it != itend; ++it) {
      std::string itLocalName, itNodeName;
      (*it)->ExtractNodeAndLocalNames(itLocalName, itNodeName);
      os << "\"" << itNodeName << "\" -> \"" << LeaderNodeName << "\""
         << std::endl
         << " [ headlabel = \"" << LeaderLocalName << "\" , taillabel = \""
         << itLocalName << "\" ]" << std::endl;
    }
  }
  return os;
}

template <class Time>
std::ostream &
TimeDependency<Time>::displayDependencies(std::ostream &os, const int depth,
                                          std::string space, std::string next1,
                                          std::string next2) const {
  leader.SignalBase<Time>::displayDependencies(os, depth, space, next1, next2)
      << " (";
  switch (dependencyType) {
  case ALWAYS_READY:
    os << "A";
    break;
  case BOOL_DEPENDENT:
    os << "ready=" << ((leader.getReady()) ? "TRUE" : "FALSE");
    break;
  case TIME_DEPENDENT:
    os << "t=" << leader.getTime() << " (/" << periodTime << ") ";
    break;
  };
  os << ")"; //<<std::endl;
  {
    const typename Dependencies::const_iterator itend = dependencies.end();
    for (typename Dependencies::const_iterator it = dependencies.begin();
         it != itend; ++it)
      if (depth != 0) {
        os << std::endl;
        std::string ajout = "|";
        std::string ajout2 = "|";
        typename Dependencies::const_iterator it2 = it;
        it2++;
        if (it2 == dependencies.end()) {
          ajout = "`";
          ajout2 = " ";
        }
        (*it)->displayDependencies(os, depth - 1, space + next2 + "   ", ajout,
                                   ajout2);
      } else {
        os << std::endl << space << "   `-- ...";
        break;
      }
  }
  return os;
}

} // end of namespace dynamicgraph

#undef __TIME_DEPENDENCY_INIT
#endif //! DYNAMIC_GRAPH_TIME_DEPENDENCY_T_CPP
