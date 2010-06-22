/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      Signal.h
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




#ifndef __SIGNAL_HH
#define __SIGNAL_HH

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <string>
#include <iostream>

#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/signal-base.h>


#ifdef HAVE_LIBBOOST_THREAD
#include <boost/thread.hpp>
#endif

namespace dynamicgraph {

/*! \ingroup dgraph
  \brief Signals link I/O ports of entities. They can be
  constant-valued signals, or copy the value of a heap variable,
  or evaluated as a function.
  See SignalPtr and SignalTimeDependent for other types of signals,
  and SignalArray for a way of grouping them.
 */
template< class T,class Time >
class Signal
: public SignalBase<Time>
{
 protected:

  enum SignalType
    {
      CONSTANT
      ,REFERENCE
      ,REFERENCE_NON_CONST
      ,FUNCTION
    };
  static const SignalType SIGNAL_TYPE_DEFAULT = CONSTANT;

  SignalType signalType;
  T Tcopy1,Tcopy2;
  T* Tcopy;
  bool copyInit;

  const T* Treference;
  T* TreferenceNonConst;
  boost::function2<T&,T&,Time> Tfunction;

  bool keepReference;
  const static bool KEEP_REFERENCE_DEFAULT = false;

 public:
#ifdef HAVE_LIBBOOST_THREAD
  typedef boost::try_mutex Mutex;
  typedef boost::lock_error MutexError;
#else
  typedef int* Mutex;
  typedef int* MutexError;
#endif
  
protected:
  Mutex *providerMutex;
  using SignalBase<Time>::signalTime;

 public:
  using SignalBase<Time>::setReady;

 public:

  /* --- Constructor/destrusctor --- */
  Signal( std::string name );
  virtual ~Signal( void ) {}

  /* --- Generic In/Out function --- */
  virtual void get( std::ostream& value ) const;
  virtual void set( std::istringstream& value ) ;
  virtual void trace( std::ostream& os ) const;

  /* --- Generic Set function --- */
  virtual void setConstant( const T& t ); 
  virtual void setReference( const T* t,Mutex *mutexref=NULL );
  virtual void setReferenceNonConstant( T* t,Mutex *mutexref=NULL );
  virtual void setFunction( boost::function2<T&,T&,Time> t,
			    Mutex *mutexref=NULL);

  inline bool getKeepReference( void ){ return keepReference; }
  inline void setKeepReference( const bool& b ){ keepReference=b; }

  /* --- Signal computation --- */
  virtual const T& access( const Time & t );
  virtual inline void recompute( const Time & t ) { access(t); }
  virtual const T& accessCopy( void ) const;
  
  virtual std::ostream& display( std::ostream& os ) const;

  /* --- Operators --- */
  virtual inline const T& operator()( const Time & t ){ return access(t); }
  virtual Signal<T,Time>& operator= ( const T& t );
  inline operator const T& ( void ) const { return accessCopy(); }


 public:
  virtual void checkCompatibility( void ) { throw Tcopy;  }

 private:
  const T& setTcopy( const T& t );
  T& getTwork( void );
  const T& getTwork( void ) const ;
  const T& switchTcopy( void );

};

} // namespace dynamicgraph

#include <dynamic-graph/signal.t.cpp>

#endif // #ifndef __SIGNAL_HH

/*
 * Local variables:
 * c-basic-offset: 4
 * End:
 */

