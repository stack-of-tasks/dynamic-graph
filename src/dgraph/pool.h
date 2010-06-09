/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      pool.h
 * Project:   SOT
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

#ifndef POOL_H_
#define POOL_H_

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- STD --- */
#include <map>
#include <string>
#include <sstream>

/* --- SOT --- */
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/dynamicGraph-API.h>

namespace dynamicgraph
{

class Entity;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/*! @ingroup dgraph
  \brief This class keep tracks of all the objects in the stack of Tasks.

  This class gives access to and remembers all the entities created
  during its life.

  This class provides the necessary operations to register, unregister each
  instance of thoses classes.
  As tasks and features derived from Entities, they should be registered
  as such.

  \note From the code it is not very clear why we should not unregister
  from the tasks and the features...

  The role of this class is also to look for the object supporting
  a command, and to apply this command.

  It also returns signal.

 */
class DYNAMICGRAPH_EXPORT PoolStorage
{
 public:
  /*! \name Define types to simplify the writing
    @{
   */
  /*! \brief Sorted set of entities with unique key (name). */
  typedef std::map< std::string,Entity* > Entities;

 protected:
  /*! \name Fields of the class to manage the three entities.
    Also the name is singular, those are true sets.
    @{
  */
  /*! \brief Set of basic objects of the SoT */
  Entities entity;

 public:
  /*! \brief Default destructor */
  ~PoolStorage( void );

  /*! \name Method related to the handling of entities.
    @{
   */
  /*! \brief Register an entity.
    \par[in] entname: The name of the entity,
    \par[in] ent: Pointer towards the entity.
  */
  void registerEntity( const std::string& entname,Entity* ent );

  /*! \brief Unregister an entity.
    \par[in] entname: The name of the entity,
  */
  void deregisterEntity( const std::string& entname );

  /*! \brief Get an entity.
    \par[in] entname: The name of the entity,
    \return Pointer towards the entity.
  */
  Entity& getEntity( const std::string& name );

  /*! \brief Disallocate an entity.
    \par[in] entname: The name of the entity,
  */
  void clearPlugin( const std::string& name );
  /*! @} */


  /*! \brief Get a reference to a signal. */
  SignalBase<int>& getSignal( std::istringstream& sigpath );

  /*! \brief This method looks for the object named objectName,
    and ask to provide the function functionName with the arguments cmdArg.
    If the method of the object displays some information this will
    be done on os.

    The commands specific to the <b>pool<\b> object are:
    \li <b>list</b> : List all the entities registered in the pool.s
  */
  void commandLine( const std::string& objectName,const std::string& functionName,
		    std::istringstream& cmdArg, std::ostream& os );

  /*! \brief This method write a graph description on the file named FileName. */
  void writeGraph(const std::string &aFileName);
  void writeCompletionList(std::ostream& os);
};

DYNAMICGRAPH_EXPORT extern dynamicgraph::PoolStorage pool;

}  // namespace dynamicgraph

#endif /* POOL_H_ */
