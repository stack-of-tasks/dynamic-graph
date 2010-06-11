/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      TimeDependancy.h
 * Project:   SOT
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




#ifndef __TIME_DEPENDANCY_HH
#define __TIME_DEPENDANCY_HH

#include <list>
#include <iostream> 

#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/signal-array.h>

template< class Time >
class TimeDependancy
{
 public:
  enum DependancyType
    {
      TIME_DEPENDANT, 
      BOOL_DEPENDANT,
      ALWAYS_READY
    };

  mutable Time lastAskForUpdate;

  // protected:
 public:

  SignalBase< Time >& leader;

  typedef std::list< const SignalBase<Time> * > Dependancies;
  static const DependancyType DEPENDANCY_TYPE_DEFAULT = TIME_DEPENDANT;

  Dependancies dependancies;
  bool updateFromAllChildren;
  static const bool ALL_READY_DEFAULT = false;

  DependancyType dependancyType;

  Time periodTime;
  static const Time PERIOD_TIME_DEFAULT = 1;

 public:

  TimeDependancy( SignalBase<Time>* sig,
		     const DependancyType dep = DEPENDANCY_TYPE_DEFAULT );
  TimeDependancy( SignalBase<Time>* sig,
		     const SignalArray_const<Time>& arr, 
		     const DependancyType dep = DEPENDANCY_TYPE_DEFAULT );
  ~TimeDependancy( void ) {}

  void addDependancy( const SignalBase<Time>& sig );
  void removeDependancy( const SignalBase<Time>& sig );
  void clearDependancy( void );
  

  virtual std::ostream & writeGraph(std::ostream &os) const;
  std::ostream& displayDependancies( std::ostream& os,const int depth=-1,
				     std::string space="",
				     std::string next1="",std::string next2="" ) const;
  
  bool needUpdate( const Time& t1 ) const;

  void setDependancyType( DependancyType dep ) { dependancyType = dep; }
  
  void setNeedUpdateFromAllChildren( const bool b = true ){ updateFromAllChildren=b; }
  bool getNeedUpdateFromAllChildren( void ) const { return updateFromAllChildren; } 

  void setPeriodTime( const Time& p ) { periodTime = p; }
  Time getPeriodTime( void ) const { return periodTime; }

};

#include <dynamic-graph/time-dependency.t.cpp>

#endif /* #ifndef __TIME_DEPENDANCY_HH */
