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

#ifndef __ENTITY_HH__
#define __ENTITY_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* DYNAMIC-GRAPH */
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/signal-array.h>
#include <dynamic-graph/dynamic-graph-api.h>

/* --- STD --- */
#include <string>
#include <map>
#include <sstream>

/* --- BOOST --- */
#include <boost/noncopyable.hpp>

/* NAMESPACE */
namespace dynamicgraph {

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/*! \ingroup dgraph
  \brief This class represents an entity, i.e. a generic
  computational unit that provides input and output signals.

  These signals link the entities together to form a complete computation graph.
  To declare a new entity, please see the DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN macro
  in g_factory.h.
  A command-line interface provided by the entity can be used by a sot shell to
  call methods from entities and display the result of their execution.
  Classes that derive from Entity can customize the command-line by overriding commandLine().
 */
class DYNAMIC_GRAPH_DLLAPI Entity
: private boost::noncopyable
{

 protected:
  std::string name;

  void entityRegistration( void );
  void entityDeregistration( void );

 public:
  Entity( const std::string& name );
  virtual ~Entity( void );

  static const std::string CLASS_NAME;
  const std::string& getName( void ) const { return name; }
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }

 protected: /* --- SIGNALS --- */
  typedef   std::map< std::string,SignalBase<int>* > SignalMap;
  SignalMap signalMap;
  void signalRegistration( const SignalArray<int>& signals );
  void signalDeregistration( const std::string& name );
 public:
  SignalBase<int>& getSignal( const std::string & signalName );
  const SignalBase<int>& getSignal( const std::string & signalName ) const;
  std::ostream& displaySignalList( std::ostream& os ) const;
  virtual std::ostream& writeGraph( std::ostream& os ) const;
  virtual std::ostream& writeCompletionList( std::ostream& os ) const;

 public: /* --- DISPLAY --- */
  virtual void display( std::ostream& os ) const;

 public: /* --- PARAMS --- */
  virtual void commandLine( const std::string& cmdLine,std::istringstream& cmdArgs,
			    std::ostream& os );

  virtual SignalBase<int>* test(void) { return 0; }
  virtual void test2( SignalBase<int>* ptr ) { return ; }

  virtual const std::string& getCommandList( void ) const;
};

DYNAMIC_GRAPH_DLLAPI std::ostream& operator<< (std::ostream& os, const dynamicgraph::Entity& ent );

} // namespace dynamicgraph
#endif /* #ifndef __ENTITY_HH__ */




