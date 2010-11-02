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

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* DYNAMIC-GRAPH */
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/plugin-loader.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/import.h>

/* --- STD --- */
using namespace std;
using namespace dynamicgraph;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

const std::string Interpreter::PROMPT_DEFAULT = "> ";

Interpreter::
Interpreter( PluginLoader* dl__ )
  : dlPtr(dl__),initDone(false)//,prompt( PROMPT_DEFAULT )
{
  registerFunction("plug",boost::bind(&Interpreter::cmdPlug,this,_1,_2,_3));
  registerFunction("new",boost::bind(&Interpreter::cmdNew,this,_1,_2,_3));
  registerFunction("destroy",boost::bind(&Interpreter::cmdDestroy,this,_1,_2,_3));
  registerFunction("run",boost::bind(&Interpreter::cmdRun,this,_1,_2,_3));
  registerFunction("loadPlugin",boost::bind(&Interpreter::cmdLoadPlugin,this,_1,_2,_3));
  registerFunction("unloadPlugin",boost::bind(&Interpreter::cmdUnloadPlugin,this,_1,_2,_3));
  registerFunction("help",boost::bind(&Interpreter::cmdHelp,this,_1,_2,_3));
  registerFunction("set",boost::bind(&Interpreter::cmdSetSignal,this,_1,_2,_3));
  registerFunction("get",boost::bind(&Interpreter::cmdGetSignal,this,_1,_2,_3));
  registerFunction("compute",boost::bind(&Interpreter::cmdComputeSignal,this,_1,_2,_3));
  registerFunction("import",boost::bind(&dynamicgraph::command::import,boost::ref(*this),_1,_2,_3));
  registerFunction("pushImportPaths",boost::bind(&dynamicgraph::command::pushImportPaths,boost::ref(*this),_1,_2,_3));
  registerFunction("popImportPaths",boost::bind(&dynamicgraph::command::popImportPaths,boost::ref(*this),_1,_2,_3));
  prompt = PROMPT_DEFAULT;
  initDone = true;
}

void Interpreter::
registerFunction( const string& funname,
		  const Interpreter::ShellBasicFunction& fun )
{
  if( initDone ) {dgDEBUG(15) << "Register " << funname << std::endl;}
  FunctionMap::iterator funkey = functionMap.find(funname);
  if( funkey != functionMap.end() ) // key does exist
    {
      if( initDone )
		{
		  dgDEBUG(15) << "!! Another function already defined with the same name. Overloading "
				   << "Funname is" <<funname.c_str() << endl;
		  throw ExceptionFactory( ExceptionFactory::FUNCTION_CONFLICT,
						 "Another function already defined with the same name. ",
						 "Funname is <%s>.",funname.c_str() );

		}
    }
  else
    {
//       dgDEBUG(10) << "Register function <"<< funname
// 		   << "> in the shell interpretor." <<endl;
      functionMap[funname] = fun;
    }
}



bool Interpreter::
deregisterFunction( const std::string& funname )
{
  return ( 0!=functionMap.erase(funname) );
}


/* --------------------------------------------------------------------- */
/* --- NEW ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
#include <dynamic-graph/factory.h>
using namespace std;
#include <dynamic-graph/entity.h>
#include <dynamic-graph/signal-base.h>

void Interpreter::
cmdPlug( const std::string& cmdLine, istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - plug <obj1.sig1> <obj2.sig2>"
	 << "\t\tPlug sig1 (producer) on sig2 (consumer)." <<endl;
      return;
    }
  string ssig1,ssig2;
  cmdArg>>ssig1>>ssig2;

  string obj1,fun1;
  string obj2,fun2;
  istringstream str1(ssig1),str2(ssig2);
  if( (!objectNameParser(str1,obj1,fun1))||(!objectNameParser(str2,obj2,fun2)) )
    {
      DG_THROW ExceptionFactory( ExceptionFactory::SYNTAX_ERROR,
				     "Plug function: syntax is plug OBJ1.SIG1 OBJ2.SIG2.",
				     "(while calling plug %s %s).",ssig1.c_str(),
				     ssig2.c_str() );
    }

  dgDEBUG(20) << "Get Ent1 <"<<obj1<<"> ."<<endl;
  Entity& ent1 = g_pool.getEntity(obj1);
  dgDEBUG(20) << "Get Sig1 <"<<fun1<<"> ."<<endl;
  SignalBase<int> &sig1 = ent1.getSignal(fun1);

  dgDEBUG(20) << "Get Ent2 <"<<obj2<<"> ."<<endl;
  Entity& ent2 = g_pool.getEntity(obj2);
  dgDEBUG(20) << "Get Sig2 <"<<fun2<<"> ."<<endl;
  SignalBase<int> &sig2 = ent2.getSignal(fun2);

  dgDEBUG(25) << "Plug..."<<endl;
  sig2.plug(&sig1);
}

void Interpreter::
cmdNew( const std::string& cmdLine, istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - new <class> <object>"
	 << "\t\t\tCreate a new entity." <<endl;
      return;
    }
  string className;
  string objName;
  cmdArg >> className >>objName;
  dgDEBUG(15) << "New <" << className<<"> requested."<<endl;
  if( g_factory.existEntity( className ) )
    {
      dgDEBUG(15) << "New entity<"<<className<<"> " <<objName<<std::endl;
      g_factory.newEntity(className,objName);
    }
  else os << "  !! Class <" << className << "> does not exist."<<endl;
}


void Interpreter::
cmdDestroy( const std::string& cmdLine, istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - destroy <object>"
	 << "\t\t\tDestroy an object." <<endl;
      return;
    }
  string objName;  cmdArg >> objName;
  dgDEBUG(15) << "Destroy <" << objName <<"> requested."<<endl;
  delete &( g_pool.getEntity( objName ) );

}


void Interpreter::
cmdLoadPlugin( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - loadPlugin <file.so> <directory>"
	 << "\t\tLoad the plugin from the specified directory." <<endl;
      return;
    }
  if( NULL!=dlPtr )
    {
     string pluginName,directory;
      cmdArg >> pluginName;
      cmdArg >> directory;
      if( directory.length() != 0 ) dlPtr->setDirectory( directory );
      dlPtr ->addPlugin( pluginName );

      try{
	dgDEBUG(15) << "Try to load  " << pluginName<< endl;
	dgDEBUG(25)<<"sotShell.dlPtr ="<< this->dlPtr <<endl;
	dlPtr->loadPlugins();
	dgDEBUG(25)<<"sotShell.dlPtr ="<< this->dlPtr <<endl;
      }catch( ExceptionAbstract& e ) { dgDEBUG(5) << "ExceptionAbstract " << e << endl; throw e; }
    }
  else { os << "!!  Dynamic loading functionalities not accessible through the shell." <<endl; }
}

void Interpreter::
cmdUnloadPlugin( const std::string& cmdLine, std::istringstream& cmdArg,
		 std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - unloadPlugin <path/file.so>"
	 << "\t\tUnload the plugin." <<endl;
      return;
    }

  dgDEBUGIN(15);
  if( NULL!=dlPtr )
    {
      string pluginName;
      cmdArg >> pluginName;
      try{
	dgDEBUG(25) << "Try short name " << pluginName << endl;
	const std::string& fullname = dlPtr->searchPlugin(pluginName);
	dgDEBUG(25) << "Full name " << fullname << endl;
	dlPtr->unloadPlugin(fullname);
      }
      catch(...)
	{
	  dgDEBUG(25) << "Full name " << pluginName << endl;
	  dlPtr->unloadPlugin(pluginName);
	}
    }
  else { os << "!!  Dynamic loading functionalities not accessible through the shell." <<endl; }

  dgDEBUGOUT(15);
}


void Interpreter::
cmdHelp( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os )
{
  dgDEBUGIN(25);
  os << "Help" <<endl;

  std::string procname; bool personalizedHelp = false;
  cmdArg >> ws;
  if( cmdArg.good() )
    {
      const std::streamoff gc = cmdArg.tellg();
      cmdArg >> procname;
      cmdArg.seekg(gc); cmdArg.clear();
      personalizedHelp = true;
      dgDEBUG(15)<< "Personalized help on <"<< procname<<">"<<gc<<endl;
    }

  bool procfund = !personalizedHelp;
  for( FunctionMap::const_iterator iter=functionMap.begin();
       iter!=functionMap.end();++iter )
    {
      if( iter->first!="help" )
	if( (! personalizedHelp) || ( procname==iter->first ) )
	  {  iter->second(cmdLine,cmdArg,os); procfund=true;  }
    }
  if(! procfund ) { os<<" *** Procedure <"<<procname<<"> not found."<<endl; }
  dgDEBUGOUT(25);
}


void Interpreter::
cmdRun( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - run <script.txt>\t\t\t\tRun the script." <<endl;
      return;
    }

  dgDEBUGIN(15);

  string filename; cmdArg>>filename;
  dgDEBUG(25) << "Script <" <<filename<<">"<<endl;

  ifstream script( filename.c_str(),ios::in );
  if(! script.is_open() )
    {
      DG_THROW ExceptionFactory( ExceptionFactory::READ_FILE,
				     "File is not open."," (while reading <%s>).",
				     filename.c_str() );
    }

  const int SIZE = 16384;
  char line[SIZE];int lineIdx;
  string name;
  try{
    for( lineIdx=1;;lineIdx++ )
      {
	dgDEBUGIN(15);

	script.getline(line,SIZE);
	if(! script.good() ) break;

	istringstream issTmp(line);
	if( issTmp >> name )
	    {
	issTmp.getline(line,SIZE);
	istringstream iss(line);

	dgDEBUG(25) << "Run <"<<name<<"> with args <"<<line<<">"<<endl;
	cmd( name,iss,os );

	dgDEBUGOUT(15);
	    }
      }
  } catch( ExceptionAbstract& exc ) {
    //FIXME: exception should be changed instead.
    std::string& msg = const_cast<std::string&>(exc.getStringMessage());
    std::stringstream oss;
    oss << " (in line " << lineIdx <<" of file <" << filename << ">)";
    msg = msg + oss.str();
    throw exc;
  }

  dgDEBUGOUT(15);

}


void Interpreter::
cmdSetSignal( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - set <obj.signal> <value>\t\t\tSet the signal to the constant value." <<endl;
      return;
    }

  dgDEBUGIN(15);

  string objname,signame;
  objectNameParser(cmdArg,objname,signame);
  Entity& obj = g_pool.getEntity(objname);
  SignalBase<int>& sig = obj.getSignal( signame );

  cmdArg >> ws;
  sig.set( cmdArg );

  dgDEBUGOUT(15);

}

void Interpreter::
cmdGetSignal( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - get <obj.signal> <value>\t\t\tGet the signal to the constant value." <<endl;
      return;
    }

  dgDEBUGIN(15);

  string objname,signame;
  objectNameParser(cmdArg,objname,signame);
  Entity& obj = g_pool.getEntity(objname);
  SignalBase<int>& sig = obj.getSignal( signame );

  os << signame << " = "; sig.get( os );

  dgDEBUGOUT(15);

}

void Interpreter::
cmdComputeSignal( const std::string& cmdLine, std::istringstream& cmdArg, std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "  - compute <obj.sig> <time>\t\t\tRecompute <sig> at time <time>  " <<endl;
      return;
    }

  dgDEBUGIN(15);

  string objname,signame;
  objectNameParser(cmdArg,objname,signame);
  Entity& obj = g_pool.getEntity(objname);
  SignalBase<int>& sig = obj.getSignal( signame );

  int time; cmdArg >> std::ws;
  if( cmdArg.good() )
    {cmdArg >> time;} else {time=0;}
  sig.recompute( time );

  dgDEBUGOUT(15);

}


/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */


void Interpreter::
cmd( const std::string& cmdLine, istringstream& cmdArg, std::ostream& os )
{
  istringstream cmdparse(cmdLine.c_str());
  string obj,fun;
  if (cmdLine.find_first_not_of(" ") == string::npos)
  {}
  else if( objectNameParser( cmdparse,obj,fun ) )
    {
   dgDEBUG(15) << "Object <" << obj<< "> function <"<<fun<<">"<<endl;
   g_pool.commandLine( obj,fun,cmdArg,os );
  }
  else
    {
     dgDEBUG(15) << "Function <" << cmdLine <<">"<< endl;
      FunctionMap::iterator funPtr = functionMap .find( cmdLine );
      if( funPtr == functionMap.end() )
	{
	  DG_THROW ExceptionFactory( ExceptionFactory::UNREFERED_FUNCTION,
					 "Unknown function."," (while calling <%s>)",
					 cmdLine.c_str() );
	}
      funPtr->second(cmdLine,cmdArg,os);
    }

}



bool Interpreter::
objectNameParser( istringstream& cmdparse,std::string& objName,std::string& funName  )
{
  const int SIZE=128;
  char buffer[SIZE];
  cmdparse >> ws;
  cmdparse.getline( buffer,SIZE,'.' );
  if(! cmdparse.good() ) // The callback is not an object method
    return false;

  objName = buffer;
  //cmdparse.getline( buffer,SIZE );
  //funName = buffer;
  cmdparse >> funName;
  return true;
}




void Interpreter::
shell( std::istream& sin, std::ostream& sout, const std::string& promptUser )
{
  while( 1 )
    {
      if( promptUser.length() ) sout << promptUser; else sout << prompt;
      string cmdLine;
      const int SIZE = 16384;
      char cmdArgs[SIZE];
      sin >>skipws>> cmdLine ;
      dgDEBUG(15) << "Cmd <" <<cmdLine<<">"<<endl;
      if( cmdLine == "exit" ) break;
      if( sin.eof() ) break;

      sin.getline( cmdArgs,SIZE-1 );

      if( sin.gcount() >= SIZE-2 )
	{
	  sout << "!! Line size exceeded" << endl;
	  do{ sin.getline( cmdArgs,SIZE-1 ); } while ( sin.gcount() >= SIZE-2 );
	  sout << cmdArgs << endl;
	}
      else
	{
	  dgDEBUG(15) << "Args <" <<cmdArgs<<">"<<endl;

	  istringstream args (cmdArgs);

	  try{ cmd(cmdLine,args,sout); }
	  catch( exception& e ) { dgDEBUG(1) << e.what(); throw; }
	  catch(...) { dgDEBUG(1) << "!! unknow!." <<endl; throw; }
	}
    }
}

ShellFunctionRegisterer::
ShellFunctionRegisterer( const std::string& funName,
			    const Interpreter::ShellBasicFunction& f)
{
  dgDEBUGIN(25);
  g_shell.registerFunction(funName,f);
  dgDEBUGOUT(25);
}


void Interpreter::writeCompletionList(std::ostream& os)
{
  for( FunctionMap::iterator iter=functionMap.begin();
       iter!=functionMap.end();iter++ )
    {
      const std::string & name = iter->first;
      os << name << std::endl;
    }



}

namespace dynamicgraph {
//! The global g_shell object.
	Interpreter g_shell;
}

