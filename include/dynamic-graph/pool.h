// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// dynamic-graph. If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_POOL_H
# define DYNAMIC_GRAPH_POOL_H
# include <map>
# include <string>
# include <sstream>

# include <dynamic-graph/fwd.hh>
# include <dynamic-graph/exception-factory.h>
# include <dynamic-graph/signal-base.h>
# include <dynamic-graph/dynamic-graph-api.h>

namespace dynamicgraph
{
  /*! @ingroup dgraph
    \brief Singleton that keeps track of all the entities.

    This class gives access to and remembers all the entities created
    during its life.

    This class provides the necessary operations to register, unregister each
    instance of thoses classes.
    As tasks and features derived from Entities, they should be registered
    as such.

    \note From the code it is not very clear why we should not unregister
    from the tasks and the features...

  */
  class DYNAMIC_GRAPH_DLLAPI PoolStorage
  {
  public:
    /*! \name Define types to simplify the writing
      @{
    */
    /*! \brief Sorted set of entities with unique key (name). */
    typedef std::map< std::string,Entity* > Entities;

    /// \brief Get unique instance of the class.
    static PoolStorage *getInstance();

    /// \brief Destroy the unique instance of the class
    static void destroy();

    /*! @} */

    /*! \brief Default destructor */
    ~PoolStorage  ();

    /*! \name Method related to the handling of entities.
      @{
    */
    /*! \brief Register an entity.
      \par[in] entname: The name of the entity,
      \par[in] ent: Pointer towards the entity.
    */
    void registerEntity (const std::string& entname, Entity* ent);

    /*! \brief Unregister an entity.
      \par[in] entname: The name of the entity,
    */
    void deregisterEntity (const std::string& entname);

    /*! \brief Unregister an entity.
      \par[in] entity: iterator in the map,
    */
    void deregisterEntity( const Entities::iterator& entity );
    /*! \brief Get an entity.
      \par[in] entname: The name of the entity,
      \return Pointer towards the entity.
    */
    Entity& getEntity (const std::string& name);

    /// Const access to entity map
    const Entities& getEntityMap () const;

    /*! \brief Test if the entity exists. */
    bool existEntity (const std::string& name);
    /*! \brief Test if the entity exists. If it does, return a pointer on it. */
    bool existEntity (const std::string& name, Entity*& ptr);

    /*! \brief Disallocate an entity.
      \par[in] entname: The name of the entity,
    */
    void clearPlugin (const std::string& name);
    /*! @} */

    ///
    /// \brief Get a signal by name
    ///
    /// \param sigpath stream containing a string of the form "entity.signal"
    SignalBase<int>& getSignal( std::istringstream& sigpath );

    /*! \brief This method write a graph description on the file named
        FileName. */
    void writeGraph (const std::string& aFileName);
    void writeCompletionList (std::ostream& os);

  protected:
    /*! \name Fields of the class to manage the three entities.
      Also the name is singular, those are true sets.
      @{
    */
    /*! \brief Set of basic objects of the SoT */
    Entities entityMap;

  private:
    PoolStorage () {}
    static PoolStorage* instance_;
  };

  inline PoolStorage& g_pool() { return *PoolStorage::getInstance(); }

}  // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_POOL_H
