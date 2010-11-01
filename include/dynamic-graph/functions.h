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

#ifndef __SHELL_FUNCTIONS_HH__
#define __SHELL_FUNCTIONS_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* DYNAMIC-GRAPH */
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/dynamic-graph-api.h>

/* --- STD --- */
#include <string>
#include <map>
#include <sstream>

/* --- BOOST --- */
#include <boost/function.hpp>
#include <boost/bind.hpp>

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
class DYNAMIC_GRAPH_DLLAPI ShellFunctions
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


#endif //#ifndef __DYNAMIC-GRAPH_SHELL_FUNCTIONS_HH__
