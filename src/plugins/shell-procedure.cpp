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

#include <dynamic-graph/shell-procedure.h>
#include <dynamic-graph/plugin-loader.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/entity.h>

#include <fstream>
using namespace std;
using namespace dynamicgraph;

void ShellProcedure::
cmdStartProcedure( const std::string& cmdLine,std::istringstream& args,std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - proc <name>"
	 << "\t\t\t\tProcedure header." <<endl;
      return;
    }

  args>>procName;
  dgDEBUG(5)<<"Proc <" <<procName<<">"<<endl;

  currentProc.clear ();
  args >> ws;
  while( args.good () )
    {
      std::string next;
      args>>next>>ws;
      currentProc.params.push_back(next);
    }
}

void ShellProcedure::
cmdContinueProcedure( const std::string& cmdLine,std::istringstream& args,std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - -> cmd args..."
	 << "\t\t\t\tProcedure body." <<endl;
      return;
    }

  std::string cmd2;
  args>>ws>>cmd2;
  dgDEBUG(5)<<"Proc <" <<procName<<">: "<<cmd2<<endl;

  Instruction ins; ins.cmd=cmd2;
  args >> ws;
  while( args.good () )
    {
      std::string next;
      int param = -1;
      args >> next >> ws;
      for (unsigned int i = 0; i < currentProc.params.size (); ++i)
	{
	  if (next == currentProc.params[i])
	    {
	      param = (int) i;
	      break;
	    }
	}
      ins.args.push_back (next);
      ins.params.push_back (param);
    }

  currentProc.instructions.push_back( ins );
}

void ShellProcedure::
cmdEndProcedure( const std::string& cmdLine,std::istringstream&, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - endproc..."
	 << "\t\t\t\tProcedure end." <<endl;
      return;
    }

  dgDEBUG(5)<<"Proc <" <<procName<<">: endproc"<<endl;
  procedureList[ procName ] = currentProc;
  currentProc.clear ();

  if( g_shell.deregisterFunction( procName ))
    {      os<< "Redefining proc <"<<procName<<">: procedure already defined. "
	     << "Overwrite it."<<endl;    }
  ShellFunctionRegisterer registration
  ( procName.c_str (),boost::bind(&ShellProcedure::cmdProcedure,
				 this,procName,_1,_2,_3) );

}

void ShellProcedure::
cmdProcedure(   const std::string& procname,
		const std::string& cmdLine,std::istringstream& args,std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os<<"  - "<<procname<<"\t\t\t\t\tUser-defined procedure"<<endl;
      args >> ws;
      if( args.good () )
	{
	  std::string argname;
	  const std::streamoff gc = args.tellg ();
	  args >> argname;
	  args.seekg(gc); args.clear ();
	  if( procname==argname )
	    {
	      /* if cmdline = "Help <procname>", then display
	       * the proc instruction. */
	      ProcedureList::iterator pair = procedureList.find( argname );
	      if( pair==procedureList.end () )
		{
		  DG_THROW ExceptionFactory( ExceptionFactory::UNREFERED_FUNCTION,
						 "Undefined procedure",
						 ": procedure <%s> not defined.",
						 argname.c_str () );
		}

	      Procedure & proc = pair->second;
	      unsigned int cmdnum=1;
	      for( std::list<Instruction>::iterator ins=proc.instructions.begin ();
		   ins!=proc.instructions.end (); ++ins )
		{
		  os<<"\t#" <<cmdnum++<<"  "<<ins->cmd; // <<" "<<ins->args <<endl;
		  for( unsigned int i=0;i<ins->args.size ();++i )
		    { os << " " << ins->args[i]; }
		  os << endl;
		}

	    }
	}
      return;
    }


  dgDEBUG(15) << " Calling procedure <" <<cmdLine<<"> " <<endl;
  ProcedureList::iterator pair = procedureList.find( cmdLine );
  if( pair==procedureList.end () )
    {
      DG_THROW ExceptionFactory( ExceptionFactory::UNREFERED_FUNCTION,
				     "Undefined procedure",
				     ": procedure <%s> not defined.",cmdLine.c_str () );
    }

  /* You need a copy here, in case the proc is removed from the
   * list by itself:
   * % proc next
   * % -> proc next
   * % -> -> echo TOTO
   * % -> endproc
   * % endproc
   */
  Procedure proc = pair->second;

  std::vector< std::string > paramValue;
  for( unsigned int i=0;i<proc.params.size ();++i )
    {
      args>>ws;
      if( args.good () )
	{
	  std::string next; args>>next>>ws;
	  paramValue.push_back( next );
	  dgDEBUG(25) << "Args : <" << next << ">"<<endl;
	}
      else { paramValue.push_back(""); }
    }

  istringstream iss; ostringstream oss;
  for( std::list<Instruction>::iterator ins=proc.instructions.begin ();
       ins!=proc.instructions.end (); ++ins )
    {
      dgDEBUG(15) << "Proc <" << cmdLine << "> : " << ins->cmd << endl;
      oss.clear (); oss.str("");
      for( unsigned int i=0;i<ins->params.size ();++i )
	{
	  int paramArg = ins->params[i];
	  if( paramArg==-1 ) oss << ins->args[i] << " ";
	  else oss << paramValue[(size_t) paramArg] << " ";
	}

      dgDEBUG(15) << " Args = " << oss.str () << endl;
      iss.str(oss.str ()); iss.clear ();
      g_shell.cmd(ins->cmd,iss,os);
    }
}

void ShellProcedure::
cmdFor( const std::string& cmdLine,std::istringstream& args,std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - for 1 5 instruction "<<endl;
      return;
    }

  std::string cmd2,idx;
  int istart,iend;

  {
    stringstream oss;

    args >> cmd2;  oss.str( cmd2 );
    const unsigned int SIZE = 32; char b1[SIZE],b2[SIZE],b3[SIZE];
    oss.getline( b1,SIZE,'=' );
    oss.getline( b2,SIZE,':' );
    oss.getline( b3,SIZE );
    dgDEBUG(15) << b1 << "/" << b2 << "/" << b3 << endl;

    idx = b1; istart = atoi(b2); iend = atoi(b3);
    args >> cmd2;

    dgDEBUG(15) << "FOR <" << idx << "> = " << istart << " TO " << iend
		 << " DO " << cmd2 <<endl;
  }

  string argsstr;
  {
    const unsigned int SIZE = 1024; char buffer[SIZE];
    args.getline( buffer,SIZE );
    argsstr = buffer;
  }
  for( int i=istart;i<=iend;++i )
    {
      istringstream iss; stringstream oss;

      std::string insp;
      istringstream issargs( argsstr );
      while( issargs.good () )
	{
	  issargs >> insp;
	  if( insp == idx ) { oss << i << " "; } else { oss<<insp<< " "; }
	}
      iss.str( oss.str () );
      g_shell.cmd(cmd2,iss,os);
    }
}

ShellProcedure sotShellProceduror;

extern "C" {
  ShellFunctionRegisterer regFun1
  ( "proc",boost::bind(&ShellProcedure::cmdStartProcedure,
		       &sotShellProceduror,_1,_2,_3) );

  ShellFunctionRegisterer regFun2
  ( "->",boost::bind(&ShellProcedure::cmdContinueProcedure,
		       &sotShellProceduror,_1,_2,_3) );
  ShellFunctionRegisterer regFun3
  ( "endproc",boost::bind(&ShellProcedure::cmdEndProcedure,
		       &sotShellProceduror,_1,_2,_3) );

  ShellFunctionRegisterer regFun4
  ( "for",boost::bind(&ShellProcedure::cmdFor,
		      _1,_2,_3) );
}

