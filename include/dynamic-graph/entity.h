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

#ifndef DYNAMIC_GRAPH_ENTITY_H
# define DYNAMIC_GRAPH_ENTITY_H
# include <iosfwd>
# include <map>
# include <sstream>
# include <string>

# include <boost/noncopyable.hpp>

# include <dynamic-graph/fwd.hh>
# include <dynamic-graph/dynamic-graph-api.h>
# include <dynamic-graph/exception-factory.h>
# include <dynamic-graph/signal-array.h>
# include <dynamic-graph/signal-base.h>

namespace dynamicgraph
{
  /// \ingroup dgraph
  ///
  /// \brief This class represents an entity, i.e. a generic
  /// computational unit that provides input and output signals.
  ///
  /// These signals link the entities together to form a complete
  /// computation graph.  To declare a new entity, please see the
  /// DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN macro in g_factory.h. A
  /// command-line interface provided by the entity can be used by a
  /// sot shell to call methods from entities and display the result
  /// of their execution.  Classes that derive from Entity can
  /// customize the command-line by overriding commandLine ().
  class DYNAMIC_GRAPH_DLLAPI Entity : private boost::noncopyable
  {
  public:
    static const std::string CLASS_NAME;

    explicit Entity (const std::string& name);
    virtual ~Entity  ();

    const std::string& getName  () const
    {
      return name;
    }

    virtual const std::string& getClassName  () const
    {
      return CLASS_NAME;
    }

    SignalBase<int>& getSignal (const std::string& signalName);
    const SignalBase<int>& getSignal (const std::string& signalName) const;
    std::ostream& displaySignalList(std::ostream& os) const;
    virtual std::ostream& writeGraph (std::ostream& os) const;
    virtual std::ostream& writeCompletionList (std::ostream& os) const;

    virtual void display (std::ostream& os) const;

    virtual void commandLine (const std::string& cmdLine,
			      std::istringstream& cmdArgs,
			      std::ostream& os);

    virtual SignalBase<int>* test ()
    {
      return 0;
    }

    virtual void test2 (SignalBase<int>*)
    {
      return ;
    }

    virtual const std::string& getCommandList () const;

    /// Return the list of command objects
    virtual std::map<const std::string, command::Command*>
      getNewStyleCommandMap();


  protected:
    typedef std::map< std::string,SignalBase<int>* > SignalMap;

    /// Add a command to Entity
    virtual void addCommand(const std::string& name,
			    command::Command* command);


    void entityRegistration ();
    void entityDeregistration ();

    void signalRegistration (const SignalArray<int>& signals);
    void signalDeregistration (const std::string& name);

    std::string name;
    SignalMap signalMap;
    std::map<const std::string, command::Command*> commandMap;
  };

  DYNAMIC_GRAPH_DLLAPI std::ostream&
  operator<< (std::ostream& os, const dynamicgraph::Entity& ent);
} // end of namespace dynamicgraph

#endif //! DYNAMIC_GRAPH_ENTITY_H
