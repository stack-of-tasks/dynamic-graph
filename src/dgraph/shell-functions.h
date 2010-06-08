/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      shell-functions.h
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



#ifndef __SHELL_FUNCTIONS_HH__
#define __SHELL_FUNCTIONS_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* SOT */
#include <dynamicGraph/exception-factory.h>
#include <dynamicGraph/interpreter.h>

/* --- STD --- */
#include <string>
#include <map>
#include <sstream>

/* --- BOOST --- */
#include <boost/function.hpp>
#include <boost/bind.hpp>

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32) 
#  if defined (shell_functions_EXPORTS)
#    define SHELLFUNCTIONS_EXPORT __declspec(dllexport)
#  else  
#    define SHELLFUNCTIONS_EXPORT __declspec(dllimport)
#  endif 
#else
#  define SHELLFUNCTIONS_EXPORT
#endif

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#define STATIC_SHELL_FUNCTION( name ) \
  static void name( const std::string cmd,std::istringstream& args, \
                       std::ostream& os )
/*! @ingroup dgraph
  \brief This plugin implements a bunch of useful shell commands.

  These include debug commands such as timing and exception catching,
  and user interaction commands such as display and beep commands.
  For more information, load the plugin and type help on a sot shell.
 */
class SHELLFUNCTIONS_EXPORT ShellFunctions
{
 public:

  STATIC_SHELL_FUNCTION( cmdLoadPlugins );
  STATIC_SHELL_FUNCTION( cmdTry );
  STATIC_SHELL_FUNCTION( cmdDisplayPlugins );
  STATIC_SHELL_FUNCTION( cmdDisplayFactory );
  STATIC_SHELL_FUNCTION( cmdCommentary );
  STATIC_SHELL_FUNCTION( cmdUnplug );
  STATIC_SHELL_FUNCTION( cmdClearPlugin );
  STATIC_SHELL_FUNCTION( cmdSignalTime );
  STATIC_SHELL_FUNCTION( cmdSynchroSignal );
  STATIC_SHELL_FUNCTION( cmdEcho );
  STATIC_SHELL_FUNCTION( cmdCopy );
  STATIC_SHELL_FUNCTION( cmdFreeze );
  STATIC_SHELL_FUNCTION( cmdSqueeze );
  STATIC_SHELL_FUNCTION( cmdEnableTrace );
  STATIC_SHELL_FUNCTION( cmdSetPrompt );
  STATIC_SHELL_FUNCTION( cmdSleep );
  STATIC_SHELL_FUNCTION( cmdBeep );
  STATIC_SHELL_FUNCTION( cmdFlagSet );
  STATIC_SHELL_FUNCTION( cmdCompletionList );
};




#endif //#ifndef __SOT_SHELL_FUNCTIONS_HH__
