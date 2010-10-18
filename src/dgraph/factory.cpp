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

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- DYNAMIC-GRAPH --- */
#include <dynamic-graph/debug.h>
#include <dynamic-graph/factory.h>

using namespace std;
using namespace dynamicgraph;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */


FactoryStorage::
~FactoryStorage( void )
{
  dgDEBUGINOUT(25);
}



/* --------------------------------------------------------------------- */
void FactoryStorage::
registerEntity( const std::string& entname,FactoryStorage::EntityConstructor_ptr ent )
{
  dgDEBUGIN(25);
  EntityMap::iterator entkey;
  if( existEntity(entname,entkey) ) // key does exist
    {
//       DG_THROW ExceptionFactory( ExceptionFactory::OBJECT_CONFLICT,
// 				 "Another entity class already defined with the same name. ",
// 				 "(while adding entity class <%s> inside the g_factory).",
// 				 entname.c_str() );
      dgERRORF("Another entity class already defined with the same name. "
		"(while adding entity class <%s> inside the factory).",
		entname.c_str() );
    }
  else
    {
       dgDEBUG(30) << "Register entity <"<< entname
		    << "> in the factory." <<std::endl;
      entityMap[entname] = ent;
    }
  dgDEBUGOUT(25);
}

void FactoryStorage::
deregisterEntity( const std::string& entname )
{
  dgDEBUGIN(25);
  EntityMap::iterator entkey;
  if(! existEntity(entname,entkey) ) // key does not exist
    {
      DG_THROW ExceptionFactory( ExceptionFactory::OBJECT_CONFLICT,
				     "Entity class not defined yet. ",
				     "(while removing entity class <%s>).",
				     entname.c_str() );
      dgERRORF(ExceptionFactory::OBJECT_CONFLICT,
		"Entity class not defined yet. "
		"(while removing entity class <%s>).",
		entname.c_str() );
    }
  else
    {
       dgDEBUG(30) << "Deregister entity <"<< entname
		    << "> from the factory." <<std::endl;
       entityMap.erase( entkey );
    }
  dgDEBUGOUT(25);
}

Entity* FactoryStorage::
newEntity( const std::string& classname,const std::string& objname )
{
  dgDEBUG(15) << "New <"<<classname<<">Entity <"<<objname<<">"<<std::endl;
  EntityMap::iterator entPtr;
  if(! existEntity(classname,entPtr) )
    {
      DG_THROW ExceptionFactory( ExceptionFactory::UNREFERED_OBJECT,
				     "Unknown entity."," (while calling new_entity <%s>)",
				     classname.c_str() );
    }
  return entPtr->second(objname);
}

bool FactoryStorage::
existEntity( const std::string& name, EntityMap::iterator& entPtr )
{
  entPtr = entityMap .find( name );
  return ( entPtr != entityMap.end() );
}
bool FactoryStorage::
existEntity( const std::string& name )
{
  EntityMap::iterator entPtr;return existEntity( name,entPtr );
}

void FactoryStorage::
listEntities(std::vector <std::string>& outList)
{
  for (EntityMap::iterator it = entityMap.begin();
       it != entityMap.end(); it++) {
    outList.push_back(it->first);
  }
}

/* --------------------------------------------------------------------- */
/* --- REGISTERERS ----------------------------------------------------- */
/* --------------------------------------------------------------------- */
EntityRegisterer::
EntityRegisterer( const std::string& entityClassName,
		     FactoryStorage::EntityConstructor_ptr maker)
  :entityName( entityClassName )
{
  dgDEBUGIN(15);
  g_factory.registerEntity(entityClassName,maker);
  dgDEBUGOUT(15);
}

EntityRegisterer::
~EntityRegisterer( void )
{
  dgDEBUGIN(15);

  g_factory.deregisterEntity(entityName);

  dgDEBUGOUT(15);
}


/* --------------------------------------------------------------------- */
/* ---  COMMAND LINE ---------------------------------------------------- */
/* --------------------------------------------------------------------- */
void FactoryStorage::
commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,
	     std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os<< "factory ";
      string cmd2; cmdArgs >> cmd2;
      if(! cmdArgs.good())
	os << " <arg>\t\t\t\taccess to the factory (help <arg> for more detail)" <<endl;
      else if( cmd2 == "list" )
	os << "list\t\t:List all available entities." << endl;
      else if( cmd2 == "listEntities" )
	os <<"listEntities\t:List available entities." << endl;
    }
  else if( cmdLine == "list" )
    {
      commandLine("listEntities",cmdArgs,os);
    }
  else if( cmdLine == "listEntities" )
    {
      os <<" List of available entities:" << endl;
      for( EntityMap::iterator iter = entityMap.begin();iter!=entityMap.end();++iter )
	{ os << "  - " << iter->first << endl; }
    }
  return;
}

namespace dynamicgraph {
	//! The global g_factory object.
	FactoryStorage g_factory;
}
