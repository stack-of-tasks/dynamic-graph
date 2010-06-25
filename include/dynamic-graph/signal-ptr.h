/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      SignalPtr.h
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




#ifndef __SIGNAL_PTR_HH
#define __SIGNAL_PTR_HH

#include <iostream>

#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/signal.h>

namespace dynamicgraph {

template< class T,class Time >
class SignalPtr 
: public virtual Signal<T,Time>
{
 public:
    using SignalBase<Time>::getName;

 protected: 
    Signal<T,Time>* signalPtr;
    bool modeNoThrow;
    bool transmitAbstract; 
    SignalBase<Time> * abstractTransmitter;
    T* transmitAbstractData;

    inline bool autoref( void ) const { return(signalPtr==this); }

 public: /* --- CONSTRUCTORS --- */

    SignalPtr( Signal<T,Time>* ptr,std::string name="" )
	: Signal<T,Time>(name)
	,signalPtr(ptr),modeNoThrow(false)
	,transmitAbstract(false)
	,abstractTransmitter(NULL) {}
    virtual ~SignalPtr( void ) { signalPtr = NULL; }
    
 public: /* --- PLUG-IN OPERATION --- */

    Signal<T,Time>* getPtr ( void ); // throw
    const Signal<T,Time>* getPtr ( void ) const; // throw
    SignalBase<Time>* getAbstractPtr ( void ); // throw
    const SignalBase<Time>* getAbstractPtr ( void ) const; // throw
    virtual void plug( SignalBase<Time>* ref );
    virtual void unplug( void ) { plug(NULL); }
    virtual bool isPluged( void ) const { return (NULL!=signalPtr); } 
    virtual SignalBase<Time>* getPluged( void ) const { return signalPtr; } 
    virtual bool isAbstractPluged( void ) const; 
    virtual const Time& getTime( void ) const;
    
    /* Equivalent operator-like definitions. */
    inline Signal<T,Time>* operator->() { return getPtr(); }
    inline const Signal<T,Time>* operator->() const { return getPtr(); }
    inline Signal<T,Time>& operator*( void ) { return *getPtr(); }
    inline const Signal<T,Time>& operator*( void ) const { return *getPtr(); }
    inline operator bool ( void ) const { return isPluged(); }

 public: /* --- INHERITANCE --- */

    virtual bool needUpdate( const Time& t ) const ;
    virtual std::ostream& writeGraph(std::ostream& os) const;
    virtual std::ostream& display( std::ostream& os ) const;

    /* For compatibility, .access() is equivalent to ->access(). For explicite
     * pointer dereference :
     * Prefere ->() to () 
     */
    virtual const T& operator()( const Time & t );
    /* Similarly, Prefere ->access to .access 
     */
    virtual const T& access( const Time & t );
    virtual const T& accessCopy( void ) const;

    inline void setConstantDefault( const T& t )
	{ Signal<T,Time>::setConstant(t); modeNoThrow = true; }
    virtual inline void setConstantDefault( void )
	{ setConstantDefault( accessCopy() ); }
    inline void unsetConstantDefault( void ){ modeNoThrow = false; }


 public:  /* --- INHERITANCE --- */
    
    /* SignalPtr could be used as a classical signal, through the normal
     * setting functions. The behavior is to plugged the signalPtr on
     * the classical mother Signal layer of the object.
     */
    virtual void setConstant( const T& t )
	{ plug(this); Signal<T,Time>::setConstant(t); }
    virtual void setReference( const T* t,typename Signal<T,Time>::Mutex *m=NULL )
	{ plug(this); Signal<T,Time>::setReference(t,m); }
    virtual void setFunction( boost::function2<T&,T&,Time> t,typename Signal<T,Time>::Mutex *m=NULL )
	{  plug(this);   Signal<T,Time>::setFunction(t,m); }

/*     template< class Provider > */
/* 	void setFunction( T& (Provider::*fun)(Time,T&),Provider& obj, */
/* 		  boost::try_mutex *mutexref=NULL ) */
/* 	{ plug(this); Signal<T,Time>::setFunction(fun,obj,mutexref); } */

    virtual inline Signal<T,Time>& operator= ( const T& t )
	{ setConstant( t ); return *this; }


    virtual std::ostream&  displayDependancies( std::ostream& os,const int depth=-1,
						std::string space="",
						std::string next1="",
						std::string next2="" ) const;
    
 protected: // Interdiction of the rest of the heritage
    
    virtual void addDependancy( const SignalBase<Time>& signal ) {}
    virtual void removeDependancy( const SignalBase<Time>& signal ) {}
    virtual void clearDependancies( void ) {}
    
};

} // namespace dynamicgraph

#include <dynamic-graph/signal-ptr.t.cpp>


#endif // #ifndef __SIGNAL_PTR_HH

/*
 * Local variables:
 * c-basic-offset: 4
 * End:
 */

