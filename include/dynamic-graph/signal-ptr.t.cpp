/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      SignalPtr.t.cpp
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




#ifndef __SIGNAL_PTR_TCPP
#define __SIGNAL_PTR_TCPP

#include <dynamic-graph/signal-ptr.h>

#undef VP_TEMPLATE_DEBUG_MODE
#define VP_TEMPLATE_DEBUG_MODE 0
#include <dynamic-graph/debug.h>

namespace dynamicgraph {

template< class T,class Time >
bool SignalPtr<T,Time>::
isAbstractPluged( void ) const 
{
  return ( (NULL!=signalPtr)||(abstractTransmitter) ); 
} 


template< class T,class Time >
Signal<T,Time>* SignalPtr<T,Time>::
getPtr ( void ) 
{
  dgTDEBUGIN(25);
  if(! isPluged() )
    DG_THROW ExceptionSignal( ExceptionSignal::NOT_INITIALIZED,
				  "In SignalPtr: SIN ptr not set.",
				  " (in signal <%s>)",getName().c_str());
  dgTDEBUGOUT(25);
  return signalPtr; 
}

template< class T,class Time >
const Signal<T,Time>* SignalPtr<T,Time>::
getPtr ( void ) const
{
  dgTDEBUGIN(25) << SignalBase<Time>::name <<"("<< isPluged() <<")"
		  << this << "->"<<signalPtr <<std::endl;
  dgTDEBUGIN(25);
  if(! isPluged() )
    { DG_THROW ExceptionSignal( ExceptionSignal::NOT_INITIALIZED,
				    "In SignalPtr: SIN ptr not set.",
				    " (in signal <%s>)",getName().c_str()); }
  dgTDEBUGOUT(25);
  return signalPtr; 
}

template< class T,class Time >
SignalBase<Time>* SignalPtr<T,Time>::
getAbstractPtr ( void ) 
{
  if(! isAbstractPluged() )
    { DG_THROW ExceptionSignal( ExceptionSignal::NOT_INITIALIZED,
				    "In SignalPtr: SIN ptr not set.",
				    " (in signal <%s>)",getName().c_str()); }
  if( NULL!=signalPtr ) return signalPtr;
  else return abstractTransmitter;
}

template< class T,class Time >
const SignalBase<Time>* SignalPtr<T,Time>::
getAbstractPtr ( void ) const
{
  if(! isAbstractPluged() )
    { DG_THROW ExceptionSignal( ExceptionSignal::NOT_INITIALIZED,
				    "In SignalPtr: SIN ptr not set.",
				    " (in signal <%s>)",getName().c_str()); }
  if( NULL!=signalPtr ) return signalPtr;
  else return abstractTransmitter;
}

template< class T,class Time >
void SignalPtr<T,Time>::
plug( SignalBase<Time>* unknown_ref ) 
{
  dgTDEBUGIN(5);
  if(! unknown_ref )
    { 
      signalPtr = NULL;
      transmitAbstract = false;
      dgTDEBUGOUT(5);
      return;
    }

  dgTDEBUG(5) << "# In  T = "<< getName() <<" ="
	       <<typeid( Signal<T,Time>::Tcopy1 ) .name() << "{ "<<std::endl;

     
  Signal<T,Time> * ref = dynamic_cast< Signal<T,Time>* > (unknown_ref);
  if( NULL==ref )
    {
      try {
	unknown_ref->checkCompatibility(); 
      }
      catch( T* t )
	{
	  dgTDEBUG(25) << "Cast THROW ok."<< std::endl;
	  Signal<T,Time>::setReference( t );
	  transmitAbstract = true; abstractTransmitter = unknown_ref;
	  transmitAbstractData = t;
	}
      catch(...)
	{
	  dgTDEBUG(25) << "Fatal error."<< std::endl;
	  transmitAbstract = false;
	  DG_THROW ExceptionSignal( ExceptionSignal::PLUG_IMPOSSIBLE,
					"Compl. Uncompatible types for plugin.",
					"(while trying to plug <%s> on <%s>).",
					unknown_ref->getName().c_str(),
					this->getName().c_str());
	}
    }
  else
    {
      dgTDEBUG(25) << "Cast ok."<< std::endl;
      transmitAbstract = false;
      signalPtr = ref; 
    }
  dgTDEBUGOUT(5);
}

template< class T,class Time >
bool SignalPtr<T,Time>::
needUpdate( const Time& t ) const
{ 
  if( (isAbstractPluged())&&(!autoref()) ){ return getAbstractPtr()->needUpdate(t);} 
  else return Signal<T,Time>::needUpdate(t);
}

template< class T,class Time >
const Time& SignalPtr<T,Time>::getTime( void ) const
{
  if(  (isAbstractPluged())&&(!autoref()) ){ return getAbstractPtr()->getTime();} 
  return Signal<T,Time>::getTime();
}


template< class T,class Time >
const T& SignalPtr<T,Time>::
operator()( const Time & t )
{
  return access(t); 
}

template< class T,class Time >
const T& SignalPtr<T,Time>::
access( const Time & t ) 
{
  dgTDEBUGIN(15);
  if( modeNoThrow&&(! isPluged())&&Signal<T,Time>::copyInit )
    {
      dgTDEBUGOUT(15);
      return Signal<T,Time>::accessCopy();
    }
  else if( autoref() )  
    {
      dgTDEBUGOUT(15);
      return Signal<T,Time>::access(t);
    }
  else if( transmitAbstract )
    {
      abstractTransmitter->recompute(t);
      dgTDEBUGOUT(15);
      return *transmitAbstractData;
    }
  else
    {
      dgTDEBUGOUT(15);
      return getPtr()->access(t);
    }
}
template< class T,class Time >
const T& SignalPtr<T,Time>::
accessCopy( void ) const
{
  if( modeNoThrow&&(! isPluged())&&Signal<T,Time>::copyInit )
    return Signal<T,Time>::accessCopy();
  else if( autoref() ) return Signal<T,Time>::accessCopy();
  else if( transmitAbstract ) return *transmitAbstractData;
  else return getPtr()->accessCopy();
}
template< class T, class Time>
std::ostream & SignalPtr<T,Time>::
writeGraph(std::ostream &os) const
{
  std::string LeaderLocalName;
  std::string LeaderNodeName;
  Signal<T,Time>::ExtractNodeAndLocalNames(LeaderLocalName,LeaderNodeName);
  if ( isAbstractPluged() && !autoref())
    {
      std::string itLocalName,itNodeName;
      getAbstractPtr()->ExtractNodeAndLocalNames(itLocalName,itNodeName);
      os << "\t\"" << itNodeName << "\" -> \"" << LeaderNodeName << "\"" << std::endl
	 << "\t [ headlabel = \"" << LeaderLocalName 
	 << "\" , taillabel = \"" << itLocalName 
	 << "\", fontsize=7, fontcolor=red ]" << std::endl;
      
    }
  return os;
}

template< class T,class Time >
std::ostream& SignalPtr<T,Time>::
display( std::ostream& os ) const
{
  dgTDEBUGIN(25) << SignalBase<Time>::name << this << "||" << isPluged() << "||"<<signalPtr;
  { Signal<T,Time>::display(os); }

  if( (isAbstractPluged())&&(!autoref()) ) 
    {
      // 	    dgTDEBUG(25) << "Display pointed."<<std::endl;
      // 	    getPtr()->display(os<<"PTR->"); 
      os << " -->-- PLUGGED";
    }
  else 
    { 
      if(! isAbstractPluged()) os << " UNPLUGGED";
      else if(autoref()) os << " AUTOPLUGGED";
    }
     
  dgTDEBUGOUT(25);
  return os;
}

template< class T,class Time >
std::ostream& SignalPtr<T,Time>::
displayDependencies( std::ostream& os,const int depth,
		     std::string space,
		     std::string next1,std::string next2 ) const
{
  dgTDEBUGIN(25);
  if( (isAbstractPluged())&&(!autoref()) )
    { getAbstractPtr()->displayDependencies(os,depth,space,next1+"-- "
				    +SignalBase<Time>::name+" -->",next2); }
  else 
    {
      SignalBase<Time>::displayDependencies(os,depth,space,next1,next2); 
    }
  dgTDEBUGOUT(25);
  return os;
}

} // namespace dynamicgraph

#endif // #ifndef __SIGNAL_PTR_TCPP

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */

