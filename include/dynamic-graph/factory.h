/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      g_factory.h
 * Project:   DYNAMIC-GRAPH
 * Author:    François Bleibel (from Nicolas Mansard)
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



#ifndef __FACTORY_HH__
#define __FACTORY_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- STD --- */
#include <map>
#include <string>

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
class DYNAMICGRAPH_EXPORT FactoryStorage
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

  void  commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,
		     std::ostream& os );


};

DYNAMICGRAPH_EXPORT extern FactoryStorage g_factory;

/* --- REGISTERER ----------------------------------------------------------- */
/* --- REGISTERER ----------------------------------------------------------- */
/* --- REGISTERER ----------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- ENTITY REGISTERER ---------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/*! A helper class to register an entity.
 *
 */
class DYNAMICGRAPH_EXPORT EntityRegisterer
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




