/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      TimeDependency.h
 * Project:   DYNAMIC-GRAPH
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/




#ifndef __TIME_DEPENDENCY_HH
#define __TIME_DEPENDENCY_HH

#include <list>
#include <iostream> 

#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/signal-array.h>

namespace dynamicgraph {


template< class Time >
class TimeDependency
{
 public:
  enum DependencyType
    {
      TIME_DEPENDENT, 
      BOOL_DEPENDENT,
      ALWAYS_READY
    };

  mutable Time lastAskForUpdate;

  // protected:
 public:

  SignalBase< Time >& leader;

  typedef std::list< const SignalBase<Time> * > Dependencies;
  static const DependencyType DEPENDENCY_TYPE_DEFAULT = TIME_DEPENDENT;

  Dependencies dependencies;
  bool updateFromAllChildren;
  static const bool ALL_READY_DEFAULT = false;

  DependencyType dependencyType;

  Time periodTime;
  static const Time PERIOD_TIME_DEFAULT = 1;

 public:

  TimeDependency( SignalBase<Time>* sig,
		     const DependencyType dep = DEPENDENCY_TYPE_DEFAULT );
  TimeDependency( SignalBase<Time>* sig,
		     const SignalArray_const<Time>& arr, 
		     const DependencyType dep = DEPENDENCY_TYPE_DEFAULT );
  ~TimeDependency( void ) {}

  void addDependency( const SignalBase<Time>& sig );
  void removeDependency( const SignalBase<Time>& sig );
  void clearDependency( void );
  

  virtual std::ostream & writeGraph(std::ostream &os) const;
  std::ostream& displayDependencies( std::ostream& os,const int depth=-1,
				     std::string space="",
				     std::string next1="",std::string next2="" ) const;
  
  bool needUpdate( const Time& t1 ) const;

  void setDependencyType( DependencyType dep ) { dependencyType = dep; }
  
  void setNeedUpdateFromAllChildren( const bool b = true ){ updateFromAllChildren=b; }
  bool getNeedUpdateFromAllChildren( void ) const { return updateFromAllChildren; } 

  void setPeriodTime( const Time& p ) { periodTime = p; }
  Time getPeriodTime( void ) const { return periodTime; }

};

} // namespace dynamicgraph


#include <dynamic-graph/time-dependency.t.cpp>

#endif /* #ifndef __TIME_DEPENDENCY_HH */
