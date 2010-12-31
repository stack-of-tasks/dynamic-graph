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

#ifndef DYNAMIC_GRAPH_FUNCTIONS_H
# define DYNAMIC_GRAPH_FUNCTIONS_H
# include <iosfwd>
# include <string>

# include <dynamic-graph/fwd.hh>
# include <dynamic-graph/dynamic-graph-api.h>

# define STATIC_SHELL_FUNCTION(name)				    \
  static void name (const std::string cmd,			    \
		    std::istringstream& args,			    \
		    std::ostream& os)

/*! @ingroup dgraph
  \brief This plugin implements a bunch of useful shell commands.

  These include debug commands such as timing and exception catching,
  and user interaction commands such as display and beep commands.
  For more information, load the plugin and type help on a sot shell.
 */
struct DYNAMIC_GRAPH_DLLAPI ShellFunctions
{
  STATIC_SHELL_FUNCTION (cmdLoadPlugins);
  STATIC_SHELL_FUNCTION (cmdTry);
  STATIC_SHELL_FUNCTION (cmdDisplayPlugins);
  STATIC_SHELL_FUNCTION (cmdDisplayFactory);
  STATIC_SHELL_FUNCTION (cmdCommentary);
  STATIC_SHELL_FUNCTION (cmdUnplug);
  STATIC_SHELL_FUNCTION (cmdClearPlugin);
  STATIC_SHELL_FUNCTION (cmdSignalTime);
  STATIC_SHELL_FUNCTION (cmdSynchroSignal);
  STATIC_SHELL_FUNCTION (cmdEcho);
  STATIC_SHELL_FUNCTION (cmdCopy);
  STATIC_SHELL_FUNCTION (cmdFreeze);
  STATIC_SHELL_FUNCTION (cmdSqueeze);
  STATIC_SHELL_FUNCTION (cmdEnableTrace);
  STATIC_SHELL_FUNCTION (cmdSetPrompt);
  STATIC_SHELL_FUNCTION (cmdSleep);
  STATIC_SHELL_FUNCTION (cmdBeep);
  STATIC_SHELL_FUNCTION (cmdFlagSet);
  STATIC_SHELL_FUNCTION (cmdCompletionList);
};

#endif //! DYNAMIC_GRAPH_FUNCTIONS_H
