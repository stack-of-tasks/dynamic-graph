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

/*! Local framework includes */
#include <dynamic-graph/entity.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/command.h>

/*! System includes */
#include <stdlib.h>
#include <sstream>


using namespace std;
using namespace dynamicgraph;
using dynamicgraph::command::Command;

void Entity::
entityRegistration  ()
{

  //sotPool.entity[name] = this;
  g_pool.registerEntity(name,this);
}

void Entity::
entityDeregistration  ()
{
  g_pool.deregisterEntity(name);
}

Entity::
Entity( const string& name__ )
  : name(name__)
{
  dgDEBUG(15) << "New entity <"<<name__<<">"<<endl;
  if( name.length ()==0 )
    {
      stringstream oss; oss << rand ();
      //name = this->getClassName(); Cannot call a virtual function from the constructor
      name+="::";
      name+=oss.str ();
    }

  entityRegistration ();
}

Entity::
~Entity  ()
{
  dgDEBUG(25) << "# In (" << name << " { " << endl;
  for (std::map<const std::string, Command*>::iterator it =
	 commandMap.begin(); it != commandMap.end(); it++) {
    delete it->second;
  }
  dgDEBUGOUT(25);
}

/* -------------------------------------------------------------------------- */
/* --- SIGNALS -------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void Entity::
signalRegistration( const SignalArray<int>& signals )
{
  for( unsigned int i=0;i<signals.getSize ();++i )
    {
      SignalBase<int>& sig = signals[i];
      //const string& signame = sig.getName ();
      istringstream iss( sig.getName () );
      const int SIZE = 128;
      char buffer[SIZE];
      while( iss.good () )
	{ iss.getline(buffer,SIZE,':'); }
      const string& signame( buffer );

      SignalMap::iterator sigkey = signalMap.find(signame);
      if( sigkey != signalMap.end () ) // key does exist
	{
	  dgERRORF( "Key %s already exist in the signalMap.",signame.c_str () );
	  if( sigkey->second!=&sig )
	    {
	      throw ExceptionFactory( ExceptionFactory::SIGNAL_CONFLICT,
					 "Another signal already defined with the same name. ",
					 "Signame is <%s>.",signame.c_str () );
	    }
	}
      else
	{
	  dgDEBUG(10) << "Register signal <"<< signame << "> for entity <"
		        << getName () << "> ."<<endl;
	  signalMap[signame] = &sig;
	}
    }
}


void Entity::
signalDeregistration( const std::string& signame )
{
  SignalMap::iterator sigkey = signalMap.find(signame);
  if( sigkey == signalMap.end () ) // key does not exist
    {
      dgERRORF( "Key %s does not exist in the signalMap.",signame.c_str () );
      throw ExceptionFactory( ExceptionFactory::UNREFERED_SIGNAL,
				 "No signal defined with the given name. ",
				 " (while erasing <%s>).",signame.c_str () );
    }
  else
    {
      dgDEBUG(10) << "Deregister signal <"<< signame << "> for entity <"
		   << getName () << "> ."<<endl;
      signalMap.erase(signame);
    }
}


#define __DG_ENTITY_GET_SIGNAL__(ITER_TYPE) \
  SignalMap::ITER_TYPE sigkey = signalMap.find(signame); \
  if( sigkey == signalMap.end () ) /* key does NOT exist */ \
    { \
      throw ExceptionFactory( ExceptionFactory::UNREFERED_SIGNAL,\
				 "The requested signal is not registered",\
				 ": %s",signame.c_str () );\
    }\
  return *(sigkey ->second) ;



SignalBase<int>& Entity::
getSignal( const string & signame )
{
  __DG_ENTITY_GET_SIGNAL__(iterator);
}

const SignalBase<int>& Entity::
getSignal( const string & signame ) const
{
  __DG_ENTITY_GET_SIGNAL__(const_iterator);
}


std::ostream& Entity::
displaySignalList( std::ostream& os ) const
{
  os << "--- <" << getName () << "> signal list: "<<endl;
  const SignalMap::const_iterator iterend=signalMap.end ();
  for( SignalMap::const_iterator iter = signalMap.begin ();iterend!=iter;++iter )
    {
      os << "    "; if( (++iter)--==iterend ) os << "`"; else os <<"|";
      os << "-- <" << *(iter->second) << endl;
    }
  return os;
}

std::ostream& Entity::
writeGraph( std::ostream& os ) const
{
  const SignalMap::const_iterator iterend=signalMap.end ();
  for( SignalMap::const_iterator iter = signalMap.begin ();iterend!=iter;++iter )
    {

      (*(iter->second)).writeGraph(os);
    }
  return os;
}

std::ostream& Entity::
writeCompletionList( std::ostream& os ) const
{
  const SignalMap::const_iterator iterend=signalMap.end ();
  for( SignalMap::const_iterator iter = signalMap.begin ();iterend!=iter;++iter )
    {

      os << getName () << "." << (*(iter->second)).shortName () << std::endl;
    }

  os << getCommandList () << std::endl;
  return os;
}

void Entity::
display( std::ostream& os ) const
{
  os<<this->getClassName ()<<": "<<name;
}

std::ostream& dynamicgraph::operator<< (std::ostream& os, const Entity& ent )
{
  ent.display(os);
  return os;
}

Entity::SignalMap Entity::getSignalMap() const
{
  return signalMap;
}

/* --- PARAMS --------------------------------------------------------------- */
/* --- PARAMS --------------------------------------------------------------- */
/* --- PARAMS --------------------------------------------------------------- */

static std::string Entity_COMMAND_LIST = "print\nsignals\nsignalDep";
const std::string& Entity::
getCommandList  () const
{
  return Entity_COMMAND_LIST;
}

void Entity::
commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "Entity : " << std::endl
	 << "  - print\t\t\tWhat d'you think?"<<endl
	 << "  - signals\t\t\tDisplay the signals list."<<endl
	 << "  - signalDep <signame> \tDisplay the dependency graph for signal signame."<<endl;
    }
  else if( cmdLine == "print")
    {
      os << *this << std::endl;
    }
  else if( cmdLine == "signals")
    {      displaySignalList(os);    }
  else if( cmdLine == "signalDep")
    {
      string sig; cmdArgs>>sig;
      cmdArgs >>  ws; int depth=-1;
      if( cmdArgs.good () ) { cmdArgs >> depth; }
      getSignal(sig) .displayDependencies( os,depth ); os<<endl;
    }
  else
    {
      try{
	SignalBase<int> & sig = getSignal( cmdLine );

	int time; cmdArgs>>ws;
	if( cmdArgs.good () )
	  {cmdArgs >> time;} else {time=0;}
	sig.recompute( time );

	os << cmdLine << " = "; sig.get( os );
      } catch( ExceptionFactory& e ) {
	switch( e.getCode () )
	  {
	  case ExceptionFactory::UNREFERED_SIGNAL:
	    DG_THROW ExceptionFactory( ExceptionFactory::UNREFERED_FUNCTION,
					   "The requested function/signal :","<%s> is "
					   "not registered.",cmdLine.c_str () );
	    break;
	  default:
	    throw;
	  }
      } catch( ... ) { throw;  }
    }

}

/// Add a command to Entity
void Entity::
addCommand(const std::string& inName, Command* command)
{
  if (commandMap.count(inName) != 0) {
    DG_THROW ExceptionFactory(ExceptionFactory::OBJECT_CONFLICT,
			      "Command " + inName +
			      " already registered in Entity.");
  }
  std::pair<const std::string, Command*> item(inName, command);
  commandMap.insert(item);
}

/// Return the list of command objects
std::map<const std::string, Command*> Entity::
getNewStyleCommandMap()
{
  return commandMap;
}

Command* Entity::
getNewStyleCommand( const std::string& commandName )
{
  if (commandMap.count(commandName) != 1)
    {
      DG_THROW ExceptionFactory(ExceptionFactory::UNREFERED_FUNCTION,
				"Command <" + commandName +
				"> is not registered in Entity.");
    }
  return commandMap[commandName];
}

