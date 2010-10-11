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

#ifndef __SIGNAL_ABSTRACT_HH
#define __SIGNAL_ABSTRACT_HH

#include <string>
#include <sstream>
#include <iostream>

#include <dynamic-graph/exception-signal.h>
#include <boost/noncopyable.hpp>

namespace dynamicgraph {

template< class Time >
class SignalBase : public boost::noncopyable
{
 protected:

  std::string name;
  Time signalTime;
  bool ready;

 public:

  virtual const Time& getTime( void ) const { return signalTime; }
  virtual void setTime( const Time& t ) { signalTime=t; }
  const bool& getReady( void ) const { return ready; }
  const std::string& getName( void ) const { return name; }

  virtual  void setPeriodTime( const Time& p ) {}
  virtual Time getPeriodTime( void ) const { return 1; }

 public:

  /* --- CONSTRUCTORS ------------------------------------------------------- */

  SignalBase( std::string name ="" )
      : name(name),signalTime(0),ready(false) {}
  virtual ~SignalBase( void ) {}


  /* --- DEPENDENCIES ------------------------------------------------------- */

  virtual void addDependency( const SignalBase<Time>& signal ) {}
  virtual void removeDependency( const SignalBase<Time>& signal ) {}
  virtual void clearDependencies( void ) {}

  virtual bool needUpdate( const Time& t ) const {return ready;}
  inline void setReady( const bool sready = true ) { ready = sready; }

  virtual std::ostream &
      writeGraph (std::ostream & os ) const
      {
	  return os;
      }

  virtual std::ostream&
      displayDependencies( std::ostream& os,const int depth=-1,
			   std::string space="",
			   std::string next1="",std::string next2="" ) const
      {
	  os << space << next1 << "-- ";
	  display(os);   return os;
      }

  /* --- PLUG --------------------------------------------------------------- */

  /* Plug the arg-signal on the <this> object. Plug-in is always
   * a descending operation (the actual <this> object will call the arg-signal
   * and not the opposite).
   */
  virtual void plug( SignalBase<Time>* sigarg )
      { DG_THROW ExceptionSignal( ExceptionSignal::PLUG_IMPOSSIBLE,
				      "Plug-in operation not possible with this signal. ",
				      "(while trying to plug %s on %s).",sigarg->getName().c_str(),
				      this->getName().c_str() ); }
  virtual void unplug( void )
      { DG_THROW ExceptionSignal( ExceptionSignal::PLUG_IMPOSSIBLE,
				      "Plug-in operation not possible with this signal. ",
				      "(while trying to unplug %s).",this->getName().c_str() );
      }
  virtual bool isPluged( void ) const { return  false; }
  virtual SignalBase<Time>* getPluged( void ) const { return NULL; }
  virtual  void setConstantDefault( void )
      { DG_THROW ExceptionSignal( ExceptionSignal::PLUG_IMPOSSIBLE,
				      "Plug-in operation not possible with this signal. ",
				      "(while trying to save %s).",this->getName().c_str() );
      }
  /* --- SET ---------------------------------------------------------------- */

  /* Generic set function. Should be reimplemented by the specific Signal.
   */
  virtual void set( std::istringstream& value )
      { DG_THROW ExceptionSignal( ExceptionSignal::SET_IMPOSSIBLE,
				      "Set operation not possible with this signal. ",
				      "(while trying to set %s).",this->getName().c_str() );
      }
  virtual void get( std::ostream& os ) const
      { DG_THROW ExceptionSignal( ExceptionSignal::SET_IMPOSSIBLE,
				      "Get operation not possible with this signal. ",
				      "(while trying to get %s).",this->getName().c_str() );
      }
  virtual inline void recompute( const Time & t)
      { DG_THROW ExceptionSignal( ExceptionSignal::SET_IMPOSSIBLE,
				      "Recompute operation not possible with this signal. ",
				      "(while trying to recompute %s).",this->getName().c_str() );
      }
  virtual void trace( std::ostream& os ) const
      { DG_THROW ExceptionSignal( ExceptionSignal::SET_IMPOSSIBLE,
				      "Trace operation not possible with this signal. ",
				      "(while trying to trace %s).",this->getName().c_str() );
      }

  /* --- DISPLAY ------------------------------------------------------------ */

  virtual std::ostream& display( std::ostream& os ) const
    { os<<"Sig:"<<name; return os;}

  std::string shortName( void ) const
      {
	  std::istringstream iss( name );
	  const int SIZE = 128;
	  char buffer[SIZE];
	  while( iss.good() )
	      { iss.getline(buffer,SIZE,':'); }
	  const std::string res( buffer );
	  return res;
      }
  /* --- Information providers --- */
  virtual void ExtractNodeAndLocalNames(std::string & LocalName, std::string & NodeName) const
      {
	  std::string fullname = this->getName();

	  size_t IdxPosLocalName = fullname.rfind(":");
	  LocalName = fullname.substr(IdxPosLocalName+1,fullname.length()-IdxPosLocalName+1);
	  size_t IdxPosNodeNameStart = fullname.find("(");
	  size_t IdxPosNodeNameEnd = fullname.find(")");
	  NodeName = fullname.substr(IdxPosNodeNameStart+1,IdxPosNodeNameEnd-IdxPosNodeNameStart-1);
	  /*  std::cout << "Finally "   << std:: endl
		    << "\tLocalName: " << LocalName << std::endl
		    << "\tNodeName: "  << NodeName  << std::endl; */

      }

  /* --- TEST TEST TEST TEST TEST TEST --- */
 public:
  virtual void checkCompatibility( void )
      {  DG_THROW ExceptionSignal( ExceptionSignal::PLUG_IMPOSSIBLE,
				       "Abstract signal not compatible with anything.",
				       "(while trying to plug <%s>).",
				       this->getName().c_str() );  }

};

/** Forward to a virtual fonction.
 */
template< class Time >
std::ostream& operator<< (std::ostream& os,const SignalBase<Time>& sig )
{ return sig.display(os); }

} // namespace dynamicgraph

#endif // #ifndef __SIGNAL_ABSTRACT_HH

/*
 * Local variables:
 * c-basic-offset: 4
 * End:
 */

