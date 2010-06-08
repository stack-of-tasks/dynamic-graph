/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      shell-procedure.h
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



#ifndef __SHELL_PROCEDURE_HH__
#define __SHELL_PROCEDURE_HH__

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
#include <list>
#include <vector>

/* --- BOOST --- */
#include <boost/function.hpp>
#include <boost/bind.hpp>

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32) 
#  if defined (sotShellProcedure_EXPORTS) 
#    define SOTSHELLPROCEDURE_EXPORT __declspec(dllexport)
#  else  
#    define SOTSHELLPROCEDURE_EXPORT __declspec(dllimport)
#  endif 
#else
#  define SOTSHELLPROCEDURE_EXPORT
#endif

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/*! @ingroup dgraph
  \brief This plugin exposes shell commands relative to the script language
  constructs such as procedures and for loops.

  For more information, load the plugin and type help on a sot shell.
 */
class SOTSHELLPROCEDURE_EXPORT sotShellProcedure
{
 public:
  struct Instruction
  {
    std::string cmd;
    std::vector<std::string> args;
    std::vector<int> params;
  };
  struct sotProcedure {
    std::list< Instruction > instructions;
    std::vector< std::string > params;
    void clear() { instructions.clear(); params.clear(); }
  };
  typedef std::map< std::string,sotProcedure > ProcedureList;

 public:
  std::string procName;
  ProcedureList procedureList;

  sotProcedure currentProc;

 public:
  
  void cmdStartProcedure( const std::string& cmd,std::istringstream& args,std::ostream& os );
  void cmdContinueProcedure( const std::string& cmd,std::istringstream& args,std::ostream& os );
  void cmdEndProcedure( const std::string& cmd,std::istringstream& args,std::ostream& os );

  void cmdProcedure( const std::string& procname,const std::string& cmd,
		     std::istringstream& args,std::ostream& os );

  static void cmdFor( const std::string& cmd,std::istringstream& args,std::ostream& os );


};




#endif //#ifndef __SOT_SHELL_PROCEDURE_HH__
