/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      signal-time-dependent.h
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




#ifndef __DEPENDENT_SIGNAL_HH
#define __DEPENDENT_SIGNAL_HH

#include <dynamic-graph/signal.h>

#include <dynamic-graph/time-dependency.h>

namespace dynamicgraph {

template< class T,class Time >
class SignalTimeDependent
: public virtual Signal<T,Time>
, public TimeDependency<Time> 
{

  //TimeDependency<Time> timeDependency;

 public:
  SignalTimeDependent( std::string name = "" );
  SignalTimeDependent( const SignalArray_const<Time> & arr
			  ,std::string name = "" );
  SignalTimeDependent( boost::function2<T&,T&,Time> t
			  ,const SignalArray_const<Time> & sig
			  ,std::string name = "" );
  
  virtual ~SignalTimeDependent( void ) {}
  
  inline const T& operator() ( const Time& t1 ){ return access(t1); }
  const T& access( const Time& t1 );
  
  virtual void addDependency( const SignalBase<Time>& signal ) ;
  virtual void removeDependency( const SignalBase<Time>& signal ) ;
  virtual void clearDependencies( void );

  std::ostream& writeGraph(std::ostream &os) const
    { return os;}

  std::ostream& displayDependencies( std::ostream& os,const int depth=-1,
				     std::string space="",
				     std::string next1="",std::string next2="" ) const
    { return TimeDependency<Time>::displayDependencies( os,depth,space,next1,next2 ); }

  
  virtual bool needUpdate( const Time& t ) const;
  virtual void setPeriodTime( const Time& p ) ;
  virtual Time getPeriodTime( void ) const;

};


/* -------------------------------------------- */


template < class T,class Time >
SignalTimeDependent<T,Time>::
SignalTimeDependent( std::string name )
    :Signal<T,Time>(name)
     ,TimeDependency<Time>(this)
    {}

template< class T,class Time>
SignalTimeDependent<T,Time>::
SignalTimeDependent( const SignalArray_const<Time> & arr
			  ,std::string name )
    :Signal<T,Time>(name)
    ,TimeDependency<Time>(this,arr)
    {}

template< class T,class Time>
SignalTimeDependent<T,Time>::
  SignalTimeDependent( boost::function2<T&,T&,Time> t
			  ,const SignalArray_const<Time> & sig
			  ,std::string name )
    :Signal<T,Time>(name)
    ,TimeDependency<Time>(this,sig)
    { setFunction(t); }
 
template< class T,class Time>
  const T& SignalTimeDependent<T,Time>::
access( const Time& t1 )
    {
      const bool up = TimeDependency<Time>::needUpdate(t1); 
      //SignalBase<Time>::setReady(false);

/*       std::cout << "Time before: "<< signalTime << " -- "   */
/*   		<< t1<< "  -> Up: "<<up <<std::endl ;   */
      if( up )
	{
	  TimeDependency<Time>::lastAskForUpdate = false;
	  const T& Tres = Signal<T,Time>::access(t1);
	  SignalBase<Time>::setReady(false);    
	  return Tres;
	}
      else { return Signal<T,Time>::accessCopy(); }
    }


template< class T,class Time>
   void SignalTimeDependent<T,Time>::
addDependency( const SignalBase<Time>& signal ) 
    { TimeDependency<Time>::addDependency(signal); }

template< class T,class Time>
   void SignalTimeDependent<T,Time>::
removeDependency( const SignalBase<Time>& signal ) 
    { TimeDependency<Time>::removeDependency(signal); }

template< class T,class Time>
   void SignalTimeDependent<T,Time>::
clearDependencies( void )
    { TimeDependency<Time>::clearDependency(); }

template< class T,class Time>
  bool SignalTimeDependent<T,Time>::
needUpdate( const Time& t ) const
{
  return TimeDependency<Time>::needUpdate(t); 
}

template< class T,class Time>
    void SignalTimeDependent<T,Time>::
setPeriodTime( const Time& p ) 
    { TimeDependency<Time>::setPeriodTime( p ); }
template< class T,class Time>
 Time SignalTimeDependent<T,Time>::
getPeriodTime( void )  const
    { return TimeDependency<Time>::getPeriodTime();  }

} // namespace dynamicgraph

#endif /* #ifndef __DEPENDENT_SIGNAL_HH */
