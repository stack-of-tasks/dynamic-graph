/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of dynamic-graph.
 * dynamic-graph is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * dynamic-graph is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DEPENDENT_SIGNAL_HH
#define __DEPENDENT_SIGNAL_HH

#include <dynamic-graph/signal.h>

#include <dynamic-graph/time-dependency.h>

namespace dynamicgraph {

/*!  \brief A type of signal that enforces a time dependency between other signals,
making sure its inputs are up to date on access, using a incrementing time tick as reference.
It works this way: for a given SignalTimeDependent S, the user manually adds dependent signals through the
use of the addDependency function. On access (calling the signal S operator () or access(Time) function),
if the dependent signals are not up-to-date, i.e. if their [last update] time is less than the
current time, their value will be access()'ed to bring them up-to-date. Thus, the value of dependent
signals can be accessed \b quickly and \b repeatedly through the accessCopy() function.
 */
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
