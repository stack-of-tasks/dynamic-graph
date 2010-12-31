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

#ifndef DYNAMIC_GRAPH_INTERPRETER_HELPER_H
# define DYNAMIC_GRAPH_INTERPRETER_HELPER_H
# include <iosfwd>
# include <string>

# include <dynamic-graph/fwd.hh>
# include <dynamic-graph/plugin-loader.h>

namespace dynamicgraph
{
  /*! @ingroup dgraph
    \brief This class implements the first level interpretor
    to control basic functionnalities of the plugins.

    It provides a shell allowing to :
    \li load plugin libraries,
    \li create instances of plugin,
    \li destroy the plugins,
    \li unload the libraries,
    \li set a signal,
    \li get a signal,
    \li compute a signal,
  */
  class DYNAMIC_GRAPH_DLLAPI InterpreterHelper
  {
  public:
    InterpreterHelper ()
    {}

    /*! \name Implements the commands.
      @{
    */
    /*! \brief Instanciate an object
      Extracts the name and the class of the object, verifies it is unique,
      and creates the instance if this is the case.
    */
    void cmdNew (const std::string& className,
		 const std::string& objName,
		 std::ostream& os);

    /*! \brief Destroy the object.
      Destroy the object objName.
    */
    void cmdDestroy (const std::string& objName,
		     std::ostream& os);

    /*! \brief Connect two signals.
      Connect the input signal obj1.signame1 to the output signal
      obj2.signame2.
      \param[in] obj1  The input object.
      \param[in] obj2  The output object.
      \param[in] signame1 The input object's signal name
      \param[in] signame2 The output object's signal name
      \param[out] os The output stream for logging and error output
    */
    void cmdPlug (const std::string& obj1, const std::string & signame1,
		  const std::string& obj2, const std::string & signame2,
		  std::ostream& os);

    /*! \brief Load a dynamic library which includes a plugin.
      Extracts the name first and the directory in second from cmdArg
      to load the dynamic library.
    */
    void cmdLoadPlugin (const std::string& directory,
			const std::string& pluginName,
			std::ostream& os);

    /*! \brief Unload a dynamic library which includes a plugin.
      Extracts the name to unload the dynamic library.
    */
    void cmdUnloadPlugin (const std::string& pluginName,
			  std::ostream& os);

    /*! \brief Set a signal [obj.signal] to a value [value]
      with cmdArg = "[obj.signal] [value]"
    */
    void cmdSetSignal (const std::string& objname,
		       const std::string& signame,
		       const std::string& cmdArg,
		       std::ostream& os);

    /*! \brief Display the value of the signal [obj.signal]
      with cmdArg = "[obj.signal]"
    */
    void cmdGetSignal (const std::string& objname,
		       const std::string& signame,
		       std::ostream& os);

    /*! \brief Compute the value of the signal [signame=obj.signal] at
      time [time] with cmdArg = "[obj.signal] [time]"
    */
    void cmdComputeSignal (const std::string& objname,
			   const std::string& signame,
			   const int &time,
			   std::ostream& os);

  protected:
    /// \brief The plugin loader.
    PluginLoader dlPtr;
  };


} // end of namespace dynamicgraph



#endif //! DYNAMIC_GRAPH_INTERPRETER_HELPER_H
