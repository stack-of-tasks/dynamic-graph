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

#ifndef __SHELL_PROCEDURE_HH__
#define __SHELL_PROCEDURE_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* DYNAMIC-GRAPH */
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/config-shell-procedure.hh>

/* --- STD --- */
#include <string>
#include <map>
#include <sstream>
#include <list>
#include <vector>

/* --- BOOST --- */
#include <boost/function.hpp>
#include <boost/bind.hpp>

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/*! @ingroup dgraph
  \brief This plugin exposes shell commands relative to the script language
  constructs such as procedures and for loops.

  For more information, load the plugin and type help on a sot shell.
 */
class DG_SHELLPROCEDURE_DLLAPI ShellProcedure
{
 public:
  struct Instruction
  {
    std::string cmd;
    std::vector<std::string> args;
    std::vector<int> params;
  };
  struct Procedure {
    std::list< Instruction > instructions;
    std::vector< std::string > params;
    void clear() { instructions.clear(); params.clear(); }
  };
  typedef std::map< std::string,Procedure > ProcedureList;

 public:
  std::string procName;
  ProcedureList procedureList;

  Procedure currentProc;

 public:

  void cmdStartProcedure( const std::string& cmd,std::istringstream& args,std::ostream& os );
  void cmdContinueProcedure( const std::string& cmd,std::istringstream& args,std::ostream& os );
  void cmdEndProcedure( const std::string& cmd,std::istringstream& args,std::ostream& os );

  void cmdProcedure( const std::string& procname,const std::string& cmd,
		     std::istringstream& args,std::ostream& os );

  static void cmdFor( const std::string& cmd,std::istringstream& args,std::ostream& os );


};




#endif //#ifndef __DYNAMIC-GRAPH_SHELL_PROCEDURE_HH__
