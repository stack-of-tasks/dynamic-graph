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

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      Signal.t.cpp
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



#ifndef __SIGNAL_TEMPLATE
#define __SIGNAL_TEMPLATE


#include <dynamic-graph/signal.h>
#include <dynamic-graph/signal-caster.h>

#undef VP_TEMPLATE_DEBUG_MODE
#define VP_TEMPLATE_DEBUG_MODE 0
#include <dynamic-graph/debug.h>

namespace dynamicgraph {

#define __SIGNAL_INIT(name,Tcpy,Tref,TrefNC,mutex)  \
     SignalBase<Time>(name)               \
     ,signalType(SIGNAL_TYPE_DEFAULT)            \
     ,Tcopy1(Tcpy)                               \
     ,Tcopy2(Tcpy)                               \
     ,Tcopy(&Tcopy1)                             \
     ,Treference(Tref)                           \
     ,TreferenceNonConst(TrefNC)                 \
     ,Tfunction()                                \
     ,keepReference( KEEP_REFERENCE_DEFAULT )    \
     ,providerMutex(mutex)

template< class T,class Time >
Signal<T,Time>::
Signal( std::string name )
  :__SIGNAL_INIT(name,T(),NULL,NULL,NULL)
{
  return;
}

/* -------------------------------------------------------------------------- */


template< class T,class Time >
void Signal<T,Time>::
set( std::istringstream& stringValue )
{
  (*this) = signal_cast<T>( stringValue );
}

template< class T,class Time >
void Signal<T,Time>::
get( std::ostream& os ) const
{
  signal_disp<T>( this->accessCopy(),os );
}

template< class T,class Time >
void Signal<T,Time>::
trace( std::ostream& os ) const
{
  try { signal_trace<T>( this->accessCopy(),os ); }
  catch DG_RETHROW
    catch (...)
      { DG_THROW ExceptionSignal( ExceptionSignal::SET_IMPOSSIBLE,
				      "TRACE operation not possible with this signal. ",
				      "(bad cast while getting value from %s).",
				      SignalBase<Time>::getName().c_str());
      }

}


/* -------------------------------------------------------------------------- */

template< class T,class Time >
const T& Signal<T,Time>::
setTcopy( const T& t )
{
  if( Tcopy==&Tcopy1 )
    {
      Tcopy2 = t;
      copyInit = true;
      Tcopy = &Tcopy2;
      return Tcopy2;
    }
  else
    {
      Tcopy1 = t;
      copyInit = true;
      Tcopy = &Tcopy1;
      return Tcopy1;
    }
}

template< class T,class Time >
T& Signal<T,Time>::
getTwork( void )
{
  if( Tcopy==&Tcopy1 ) return Tcopy2; else return Tcopy1;
}

template< class T,class Time >
const T& Signal<T,Time>::
getTwork( void ) const
{
  if( Tcopy==&Tcopy1 ) return Tcopy2; else return Tcopy1;
}


template< class T,class Time >
const T& Signal<T,Time>::
switchTcopy( void )
{
  if( Tcopy==&Tcopy1 )
    {
      Tcopy = &Tcopy2;
      return Tcopy2;
      }
  else
    {
      Tcopy = &Tcopy1;
      return Tcopy1;
    }
}



template< class T,class Time >
void Signal<T,Time>::
setConstant( const T& t )
{
  signalType = CONSTANT; setTcopy(t);
  setReady();
}


template< class T,class Time >
void Signal<T,Time>::
setReference( const T* t,Mutex *mutexref )
{
  signalType = REFERENCE; Treference = t;
  providerMutex = mutexref;
  copyInit = false;
  setReady();
}

template< class T,class Time >
void Signal<T,Time>::
setReferenceNonConstant( T* t,Mutex *mutexref )
{
  signalType = REFERENCE_NON_CONST;
  Treference = t;
  TreferenceNonConst = t;
  providerMutex = mutexref;
  copyInit = false;
  setReady();
}

template< class T,class Time >
void Signal<T,Time>::
setFunction( boost::function2<T&,T&,Time> t,Mutex *mutexref )
{
  signalType = FUNCTION;
  Tfunction = t;
  providerMutex = mutexref;
  copyInit = false;
  setReady();
}

template< class T,class Time >
const T& Signal<T,Time>::
accessCopy( void ) const
{
  return *Tcopy;
}



template< class T,class Time >
const T& Signal<T,Time>::
access(const Time& t)
{
  //dgTDEBUG(20) << "# In (" << SignalBase<Time>::name <<")"<<std::endl;
  switch( signalType )
    {
    case REFERENCE:
    case REFERENCE_NON_CONST:
      {
	if( NULL==providerMutex )
	  {
	    copyInit=true;
	    signalTime = t;
	    return setTcopy( *Treference );
	  }
	else
	  {
	    try
	      {
#ifdef HAVE_LIBBOOST_THREAD
		boost::try_mutex::scoped_try_lock lock(*providerMutex);
#endif
		copyInit = true;
		signalTime = t;
		return setTcopy(*Treference);
	      }
	    catch( MutexError le )
	      { return accessCopy(); }
	  }

	break;
      }

    case FUNCTION:
      {
	if( NULL==providerMutex )
	  {
	    signalTime = t;
	    Tfunction(getTwork(),t);
	    copyInit=true;
	    return switchTcopy();
	  }
	else
	  {
	    try
	      {
#ifdef HAVE_LIBBOOST_THREAD
		boost::try_mutex::scoped_try_lock lock(*providerMutex);
#endif
		signalTime = t;
		Tfunction(getTwork(),t);
		copyInit = true;
		return switchTcopy();
	      }
	    catch( MutexError le )
	      { return accessCopy(); }
	  }
	break;
      }
    case CONSTANT:
    default:
      if( this->getReady() )
	{
	  setReady(false);
	  setTime(t);
	}
      return accessCopy();
    };
}



template< class T,class Time >
Signal<T,Time>& Signal<T,Time>::operator= ( const T& t )
{
  if( keepReference&&(REFERENCE_NON_CONST==signalType)&&(NULL!=TreferenceNonConst) )
    {
      if( NULL==providerMutex )
	{
	  setTcopy( t );
	  (*TreferenceNonConst)=t;
	}
      else
	{
	  try
	    {
#ifdef HAVE_LIBBOOST_THREAD
	      boost::try_mutex::scoped_try_lock lock(*providerMutex);
#endif
	      setTcopy(t);
	      (*TreferenceNonConst)=t;
	    }
	  catch( MutexError le )
	    { /* TODO ERROR */ }
	}
    }
  else
    { setConstant( t ); }
  return *this;
}


template< class T,class Time >
std::ostream& Signal<T,Time>::
display (std::ostream& os) const
{
  os<<"Sig:"<<this->name<<" (Type ";
  switch( this->signalType )
    {
    case Signal<T,Time>::CONSTANT: os<< "Cst";break;
    case Signal<T,Time>::REFERENCE: os<<"Ref"; break;
    case Signal<T,Time>::REFERENCE_NON_CONST: os<<"RefNonCst"; break;
    case Signal<T,Time>::FUNCTION: os<<"Fun"; break;
    }
  return os<<")";
}

} // namespace dynamicgraph

#endif // #ifndef __SIGNAL_TEMPLATE

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */

