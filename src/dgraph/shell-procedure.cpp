/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      shell-procedure.cpp
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

#include <dynamicGraph/shell-procedure.h>
#include <dynamicGraph/plugin-loader.h>
#include <dynamicGraph/factory.h>
#include <dynamicGraph/debug.h>
#include <dynamicGraph/entity.h>

#include <fstream>
using namespace std;
using namespace dynamicgraph;

void sotShellProcedure::
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
  
  currentProc.clear();
  args >> ws;
  while( args.good() )
    {
      std::string next; 
      args>>next>>ws;
      currentProc.params.push_back(next);
    }
}

void sotShellProcedure::
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
//   const unsigned int SIZE = 256;
//   char buffer[SIZE]; args.getline(buffer,SIZE);
//   dgDEBUG(5)<<"Proc <" <<procName<<"> arg= ["<<buffer<<"]"<<endl;
//   ins.args=buffer;
  args >> ws;
  while( args.good() )
    {
      std::string next; int param=-1;
      args>>next>>ws;
      for( unsigned int i=0;i<currentProc.params.size();++i )
	{ if( next==currentProc.params[i] ) { param=i; break; } }
      ins.args.push_back(next);
      ins.params.push_back( param );
    }

  currentProc.instructions.push_back( ins );
}

void sotShellProcedure::
cmdEndProcedure( const std::string& cmdLine,std::istringstream& args,std::ostream& os )
{
  if( cmdLine == "help" ) 
    {
      os << "  - endproc..."
	 << "\t\t\t\tProcedure end." <<endl;
      return;
    }

  dgDEBUG(5)<<"Proc <" <<procName<<">: endproc"<<endl;
  procedureList[ procName ] = currentProc;

//   std::string toto="toto";
//   for( sotProcedure::iterator ins=procedureList[ toto ].begin();
//        ins!=procedureList[ toto ].end(); ++ins )
//     {
//       dgDEBUG(15) << "Proc <" << procName << "> : " 
// 		   << ins->cmd << " -> " << ins->args <<endl;
//     }

  currentProc.clear();

  if( Shell.deregisterFunction( procName ))
    {      os<< "Redefining proc <"<<procName<<">: procedure already defined. "
	     << "Overwrite it."<<endl;    }
  ShellFunctionRegisterer registration
  ( procName.c_str(),boost::bind(&sotShellProcedure::cmdProcedure,
				 this,procName,_1,_2,_3) );

}

void sotShellProcedure::
cmdProcedure(   const std::string& procname,
		const std::string& cmdLine,std::istringstream& args,std::ostream& os )
{
//   if( cmdLine == "help" ) 
//     {
//       std::string procname; args >> procname; os<< procname<<endl;
//       if(! args.good() )
// 	{
// 	  os << "  - User defined procedures: "<<endl;
// 	  for( ProcedureList::iterator iter=procedureList.begin();
// 	       iter!=procedureList.end(); ++iter )
// 	    os << "     -> " << iter->first << endl;
// 	  return;
// 	}
//       else
// 	{
// 	  os <<"Help on user defined procedure <" << procname << ">"<<std::endl;
// 	}
//     }
  if( cmdLine == "help" ) 
    {
      os<<"  - "<<procname<<"\t\t\t\t\tUser-defined procedure"<<endl;
      args >> ws; 
      if( args.good() ) 
	{
	  std::string argname;
	  const unsigned int gc = args.tellg(); 
	  args >> argname; 
	  args.seekg(gc); args.clear();
	  if( procname==argname )
	    {
	      /* if cmdline = "Help <procname>", then display
	       * the proc instruction. */
	      ProcedureList::iterator pair = procedureList.find( argname );
	      if( pair==procedureList.end() )
		{
		  DG_THROW ExceptionFactory( ExceptionFactory::UNREFERED_FUNCTION,
						 "Undefined procedure",
						 ": procedure <%s> not defined.",
						 argname.c_str() );
		}
	      
	      sotProcedure & proc = pair->second;
	      unsigned int cmdnum=1;
	      for( std::list<Instruction>::iterator ins=proc.instructions.begin();
		   ins!=proc.instructions.end(); ++ins )
		{
		  os<<"\t#" <<cmdnum++<<"  "<<ins->cmd; // <<" "<<ins->args <<endl;
		  for( unsigned int i=0;i<ins->args.size();++i )
		    { os << " " << ins->args[i]; }
		  os << endl;
		}

	    }
	}   
      return;
    }


  dgDEBUG(15) << " Calling procedure <" <<cmdLine<<"> " <<endl;
  ProcedureList::iterator pair = procedureList.find( cmdLine );
  if( pair==procedureList.end() )
    {
      DG_THROW ExceptionFactory( ExceptionFactory::UNREFERED_FUNCTION,
				     "Undefined procedure",
				     ": procedure <%s> not defined.",cmdLine.c_str() );
    }

  /* You need a copy here, in case the proc is removed from the
   * list by itself:
   * % proc next
   * % -> proc next
   * % -> -> echo TOTO
   * % -> endproc
   * % endproc
   */
  sotProcedure proc = pair->second;

  std::vector< std::string > paramValue;
  for( unsigned int i=0;i<proc.params.size();++i )
    { 
      args>>ws;
      if( args.good() )
	{ 
	  std::string next; args>>next>>ws; 
	  paramValue.push_back( next );
	  dgDEBUG(25) << "Args : <" << next << ">"<<endl;
	}
      else { paramValue.push_back(""); }
    }

  istringstream iss; ostringstream oss; 
  for( std::list<Instruction>::iterator ins=proc.instructions.begin();
       ins!=proc.instructions.end(); ++ins )
    {
      dgDEBUG(15) << "Proc <" << cmdLine << "> : " << ins->cmd << endl;
      oss.clear(); oss.str("");
      for( unsigned int i=0;i<ins->params.size();++i )
	{
	  int paramArg = ins->params[i];
	  if( paramArg==-1 ) oss << ins->args[i] << " ";
	  else oss << paramValue[paramArg] << " ";
	}
      
      dgDEBUG(15) << " Args = " << oss.str() << endl;
      iss.str(oss.str()); iss.clear();
      Shell.cmd(ins->cmd,iss,os);
    }
}

void sotShellProcedure::
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
      while( issargs.good() )
	{
	  issargs >> insp;
	  if( insp == idx ) { oss << i << " "; } else { oss<<insp<< " "; }
	}
      iss.str( oss.str() );
      Shell.cmd(cmd2,iss,os);
    }
}

sotShellProcedure sotShellProceduror;

extern "C" {
  ShellFunctionRegisterer regFun1
  ( "proc",boost::bind(&sotShellProcedure::cmdStartProcedure,
		       &sotShellProceduror,_1,_2,_3) );

  ShellFunctionRegisterer regFun2
  ( "->",boost::bind(&sotShellProcedure::cmdContinueProcedure,
		       &sotShellProceduror,_1,_2,_3) );
  ShellFunctionRegisterer regFun3
  ( "endproc",boost::bind(&sotShellProcedure::cmdEndProcedure,
		       &sotShellProceduror,_1,_2,_3) );

  ShellFunctionRegisterer regFun4
  ( "for",boost::bind(&sotShellProcedure::cmdFor,
		      _1,_2,_3) );
}                                                       

