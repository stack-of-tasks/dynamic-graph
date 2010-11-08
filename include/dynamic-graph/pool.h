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

#ifndef POOL_H_
#define POOL_H_

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- STD --- */
#include <map>
#include <string>
#include <sstream>

/* --- DYNAMIC-GRAPH --- */
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/dynamic-graph-api.h>

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
class DYNAMIC_GRAPH_DLLAPI PoolStorage
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

    The commands specific to the \b g_pool object are:
    \li \b list : List all the entities registered in the pool
  */
  void commandLine( const std::string& objectName,const std::string& functionName,
		    std::istringstream& cmdArg, std::ostream& os );

  /*! \brief This method write a graph description on the file named FileName. */
  void writeGraph(const std::string &aFileName);
  void writeCompletionList(std::ostream& os);
};

DYNAMIC_GRAPH_DLLAPI extern dynamicgraph::PoolStorage g_pool;

}  // namespace dynamicgraph

#endif /* POOL_H_ */
