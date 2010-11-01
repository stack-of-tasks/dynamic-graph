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

#ifndef __FACTORY_HH__
#define __FACTORY_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- STD --- */
#include <map>
#include <string>
#include <vector>
/* --- DYNAMIC-GRAPH --- */
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/dynamic-graph-api.h>

namespace dynamicgraph {

class Entity;

/* --------------------------------------------------------------------- */
/* --- FACTORY ---------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/*! \class FactoryStorage
 *  \ingroup dgraph
 *  \brief The Factory class is responsible for creating Entity objects.
 *
 *  Entities can register themselves through the helper class EntityRegisterer.
 *  This object also provides access to command-line functions.
 *
 *
 */
class DYNAMIC_GRAPH_DLLAPI FactoryStorage
{
 public:

  typedef Entity* (*EntityConstructor_ptr)( const std::string& );

 protected:
  typedef std::map< std::string,EntityConstructor_ptr > EntityMap;

  EntityMap entityMap;

 public:

  ~FactoryStorage( void );

  void registerEntity( const std::string& entname,EntityConstructor_ptr ent );
  void deregisterEntity( const std::string& entname );
  Entity* newEntity( const std::string& name,const std::string& objname );
  bool existEntity( const std::string& name, EntityMap::iterator& entPtr );
  bool existEntity( const std::string& name );
  /// Return the list of Entity class names registered in the factory.
  /// Class names are appended at end of output vector.
  void listEntities(std::vector <std::string>& outList);
  void  commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,
		     std::ostream& os );


};

DYNAMIC_GRAPH_DLLAPI extern FactoryStorage g_factory;

/* --- REGISTERER ----------------------------------------------------------- */
/* --- REGISTERER ----------------------------------------------------------- */
/* --- REGISTERER ----------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- ENTITY REGISTERER ---------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/*! A helper class to register an entity.
 *
 */
class DYNAMIC_GRAPH_DLLAPI EntityRegisterer
{
 private:
  EntityRegisterer( void );
  std::string entityName;

 public:
  EntityRegisterer( const std::string& entityClassName,
		       FactoryStorage::EntityConstructor_ptr maker);

  ~EntityRegisterer( void );
};

/*! This macro should be used to automatically register an entity
 * of classType to the g_factory. It is then possible to create it
 * with the g_factory.
 */
#define DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(classType,className) \
  const std::string classType::CLASS_NAME = className; \
  extern "C" {                                            \
    Entity *EntityMaker##_##classType( const std::string& objname )    \
    {                                                     \
      return new classType( objname );                 \
    }                                                     \
  EntityRegisterer reg##_##classType( className,    \
					    &EntityMaker##_##classType );   \
  }                                                       \

}  // namespace dynamicgraph

#endif /* #ifndef __FACTORY_HH__ */




