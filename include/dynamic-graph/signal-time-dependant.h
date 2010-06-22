/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      signal-time-dependant.h
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




#ifndef __DEPENDANT_SIGNAL_HH
#define __DEPENDANT_SIGNAL_HH

#include <dynamic-graph/signal.h>

#include <dynamic-graph/time-dependency.h>

namespace dynamicgraph {

template< class T,class Time >
class SignalTimeDependant
: public virtual Signal<T,Time>
, public TimeDependancy<Time> 
{

  //TimeDependancy<Time> timeDependancy;

 public:
  SignalTimeDependant( std::string name = "" );
  SignalTimeDependant( const SignalArray_const<Time> & arr
			  ,std::string name = "" );
  SignalTimeDependant( boost::function2<T&,T&,Time> t
			  ,const SignalArray_const<Time> & sig
			  ,std::string name = "" );
  
  virtual ~SignalTimeDependant( void ) {}
  
  inline const T& operator() ( const Time& t1 ){ return access(t1); }
  const T& access( const Time& t1 );
  
  virtual void addDependancy( const SignalBase<Time>& signal ) ;
  virtual void removeDependancy( const SignalBase<Time>& signal ) ;
  virtual void clearDependancies( void );

  std::ostream& writeGraph(std::ostream &os) const
    { return os;}

  std::ostream& displayDependancies( std::ostream& os,const int depth=-1,
				     std::string space="",
				     std::string next1="",std::string next2="" ) const
    { return TimeDependancy<Time>::displayDependancies( os,depth,space,next1,next2 ); }

  
  virtual bool needUpdate( const Time& t ) const;
  virtual void setPeriodTime( const Time& p ) ;
  virtual Time getPeriodTime( void ) const;

};


/* -------------------------------------------- */


template < class T,class Time >
SignalTimeDependant<T,Time>::
SignalTimeDependant( std::string name )
    :Signal<T,Time>(name)
     ,TimeDependancy<Time>(this)
    {}

template< class T,class Time>
SignalTimeDependant<T,Time>::
SignalTimeDependant( const SignalArray_const<Time> & arr
			  ,std::string name )
    :Signal<T,Time>(name)
    ,TimeDependancy<Time>(this,arr)
    {}

template< class T,class Time>
SignalTimeDependant<T,Time>::
  SignalTimeDependant( boost::function2<T&,T&,Time> t
			  ,const SignalArray_const<Time> & sig
			  ,std::string name )
    :Signal<T,Time>(name)
    ,TimeDependancy<Time>(this,sig)
    { setFunction(t); }
 
template< class T,class Time>
  const T& SignalTimeDependant<T,Time>::
access( const Time& t1 )
    {
      const bool up = TimeDependancy<Time>::needUpdate(t1); 
      //SignalBase<Time>::setReady(false);

/*       std::cout << "Time before: "<< signalTime << " -- "   */
/*   		<< t1<< "  -> Up: "<<up <<std::endl ;   */
      if( up )
	{
	  TimeDependancy<Time>::lastAskForUpdate = false;
	  const T& Tres = Signal<T,Time>::access(t1);
	  SignalBase<Time>::setReady(false);    
	  return Tres;
	}
      else { return Signal<T,Time>::accessCopy(); }
    }


template< class T,class Time>
   void SignalTimeDependant<T,Time>::
addDependancy( const SignalBase<Time>& signal ) 
    { TimeDependancy<Time>::addDependancy(signal); }

template< class T,class Time>
   void SignalTimeDependant<T,Time>::
removeDependancy( const SignalBase<Time>& signal ) 
    { TimeDependancy<Time>::removeDependancy(signal); }

template< class T,class Time>
   void SignalTimeDependant<T,Time>::
clearDependancies( void )
    { TimeDependancy<Time>::clearDependancy(); }

template< class T,class Time>
  bool SignalTimeDependant<T,Time>::
needUpdate( const Time& t ) const
{
  return TimeDependancy<Time>::needUpdate(t); 
}

template< class T,class Time>
    void SignalTimeDependant<T,Time>::
setPeriodTime( const Time& p ) 
    { TimeDependancy<Time>::setPeriodTime( p ); }
template< class T,class Time>
 Time SignalTimeDependant<T,Time>::
getPeriodTime( void )  const
    { return TimeDependancy<Time>::getPeriodTime();  }

} // namespace dynamicgraph

#endif /* #ifndef __DEPENDANT_SIGNAL_HH */
