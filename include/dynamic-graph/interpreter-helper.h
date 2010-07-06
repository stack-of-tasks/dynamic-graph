/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2010
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      interpreter.h
 * Project:   DYNAMIC-GRAPH
 * Author:    O. Stasse, 
 *            F. Bleibel, 
 *            N. Mansard
 *
 * Description
 * ============
 * \file This class provides an entry point for any interpreter
 *       willing to connect to the dynamic-graph.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



#ifndef __INTERPRETER_HELPER_HH__
#define __INTERPRETER_HELPER_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* DYNAMIC-GRAPH */
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/plugin-loader.h>

/* --- STD --- */
#include <string>
#include <map>
#include <sstream>

/* --- BOOST --- */
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace dynamicgraph {
/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

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
class DYNAMICGRAPH_EXPORT InterpreterHelper
{

 public:
  /*! \brief Default constructor
    \par[in] dlPtr: a plugin loader to perform the actions provided by this shell.
   */
  InterpreterHelper( );


 protected:

  /*! \brief The plugin loader */
  PluginLoader dlPtr;

 public:

  /*! \name Implements the commands.
    @{
   */
  /*! \brief Instanciante an object
    Extracts the name and the class of the object, verifies it is unique,
    and creates the instance if this is the case.
   */
  void cmdNew(const std::string& className,
	      const std::string& objName,
	      std::ostream& os);

  /*! \brief Destroy the object.
    Destroy the object objName.
   */
  void cmdDestroy( const std::string& objName, 
		   std::ostream& os );

  /*! \brief Connect two signals.
    Connect the input signal obj1.signame1 to the output signal
    obj2.signame2.
    \param[in] obj1.signame1: the input signal.
    \param[in] obj2.signame2: the output signal.
  */
  void cmdPlug( const std::string& obj1, const std::string & signame1,
		const std::string& obj2, const std::string & signame2,
		std::ostream& os  );

  /*! \brief Load a dynamic library which includes a plugin.
    Extracts the name first and the directory in second from cmdArg
    to load the dynamic library.
   */
  void cmdLoadPlugin( const std::string& directory, 
		      const std::string& pluginName, 
		      std::ostream& os );

  /*! \brief Unload a dynamic library which includes a plugin.
    Extracts the name to unload the dynamic library.
  */
  void cmdUnloadPlugin( const std::string& pluginName, 
			std::ostream& os );

  /*! \brief Set a signal <obj.signal> to a value <value>
    with cmdArg = "<obj.signal> <value>"
   */
  void cmdSetSignal( const std::string& objname, 
		     const std::string& signame,
		     const std::string& cmdArg,
		     std::ostream& os);

  /*! \brief Display the value of the signal <obj.signal>
    with cmdArg = "<obj.signal>"
   */
  void cmdGetSignal( const std::string& objname, 
		     const std::string& signame, 
		     std::ostream& os);

  /*! \brief Compute the value of the signal <obj.signal> at time <time>
    with cmdArg = "<obj.signal> <time>"
   */
  void cmdComputeSignal( const std::string& objname,
			 const std::string& signame, 
			 const int &time,
			 std::ostream& os );

};


} // namespace dynamicgraph



#endif /* #ifndef  */




