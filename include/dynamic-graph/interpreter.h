/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      interpreter.h
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



#ifndef __INTERPRETER_HH__
#define __INTERPRETER_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* SOT */
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/dynamic-graph-api.h>

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
class PluginLoader;

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
  \li run scripts,
  \li compute a signal,
  \li act as a proxy for the commands of the plugins.
*/
class DYNAMICGRAPH_EXPORT Interpreter
{

 public:
  /*! \brief Type of a basic function for the shell.
    Such function returns a void and takes 3 arguments:
    \li the functionnality (i.e. the method) it provides to the shell,
    \li a stream of input string to pass when called,
    \li an output stream for display.
  */
  typedef boost::function3<void,const std::string&,std::istringstream&,std::ostream&> ShellBasicFunction;
 protected:
  /*! \brief This type defines a sorted set of basic functions with a unique key. */
  typedef std::map< std::string,ShellBasicFunction > FunctionMap;
  /*! Instance of this type. */
  FunctionMap functionMap;

 public:
  /*! \brief Default constructor
    \par[in] dlPtr: a plugin loader to perform the actions provided by this shell.
   */
  Interpreter( PluginLoader* dlPtr=NULL );

  typedef PluginLoader * sotPluginLoaderPTR;
  /*! \brief The plugin loader */
  sotPluginLoaderPTR dlPtr;

 protected:
  bool initDone; // Debug
 public:
  std::string prompt;
  static const std::string PROMPT_DEFAULT;

 public:
  /*! \brief Register a function at the upper level of the shell.
    \par[in] funname: The function's name,
    \par[in] fun: The pointer to the function.
  */
  void registerFunction( const std::string& funname, const ShellBasicFunction& fun );
  bool deregisterFunction( const std::string& funname );


  /*! \brief Return the reference to the plugin loader. */
  void referencePluginLoader( PluginLoader* dl__ )
  { dlPtr = dl__; }

 public:

  /*! \name Implements the commands.
    @{
   */
  /*! \brief Instanciante an object
    Extracts the name and the class of the object, verifies it is unique,
    and creates the instance if this is the case.
   */
  void cmdNew( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );
  /*! \brief Destroy the object.
    Extracts the name for cmdArg and destroy the object.
   */
  void cmdDestroy( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );
  /*! \brief Connect two signals.
    Connect an input signal to an output one.
  */
  void cmdPlug( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );
  /*! \brief Load a dynamic library which includes a plugin.
    Extracts the name first and the directory in second from cmdArg
    to load the dynamic library.
   */
  void cmdLoadPlugin( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Unload a dynamic library which includes a plugin.
    Extracts the name to unload the dynamic library.
  */
  void cmdUnloadPlugin( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Ask to each register function to display its help. */
  void cmdHelp( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Run a script */
  void cmdRun( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Set a signal <obj.signal> to a value <value>
    with cmdArg = "<obj.signal> <value>"
   */
  void cmdSetSignal( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Display the value of the signal <obj.signal>
    with cmdArg = "<obj.signal>"
   */
  void cmdGetSignal( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Compute the value of the signal <obj.signal> at time <time>
    with cmdArg = "<obj.signal> <time>"
   */
  void cmdComputeSignal( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Entry point to treat a command. */
  void cmd( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os );

  /*! \brief Extract the object name and the function name from a command. */
  static bool objectNameParser( std::istringstream& cmdparse,
				std::string& objName,std::string& funName );

  /*! @} */

 public:
  /*! \brief Method to start the shell.
    \par[in] sin: The input stream from which the command will be extracted.
    \par[out] sout: The output stream to which the result will be displayed.
    \par[in] prompt: The prompt to be displayed.

   */
  void shell( std::istream& sin, std::ostream& sout, const std::string& prompt="" );
  void writeCompletionList(std::ostream& os);


};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/*! \brief Class for automatic registration of shell functions.
 * Use the macro SOT_SHELL_FUNCTION_REGISTERER(name, fun) in a .cpp file
 * to register functions on plugin load.
 */
class DYNAMICGRAPH_EXPORT ShellFunctionRegisterer

{
public:
	ShellFunctionRegisterer( const std::string& funName,
			      const Interpreter::ShellBasicFunction& f);
};

#define SHELL_FUNCTION_REGISTERER(name,fun)           \
  extern "C" {                                            \
	ShellFunctionRegisterer reg( name,boost::bind(fun,_1,_2,_3) );\
  }

DYNAMICGRAPH_EXPORT extern Interpreter Shell;

} // namespace dynamicgraph



#endif /* #ifndef  */




