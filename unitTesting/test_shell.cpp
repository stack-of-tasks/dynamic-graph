/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-JAPAN, Tsukuba, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      test_shell.cc
 * Project:   SOT
 * Author:    Nicolas Mansard
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

/* -------------------------------------------------------------------------- */
/* --- INCLUDES ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#include <dynamic-graph/entity.h>
#include <dynamic-graph/plugin-loader.h>
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/debug.h>

#include <sstream>
using namespace std;
using namespace dynamicgraph;


extern std::ofstream debugfile;

int main( int argc,char** argv )
{
  dgDEBUGIN(15);
  
  dgDEBUG(5) << " Loading..." << endl;
  PluginLoader pl;
  Shell.referencePluginLoader( &pl );

  int fileIdx;
  try
    {
      for( fileIdx=1;fileIdx<argc;++fileIdx )
	{
	  std::istringstream script( argv[fileIdx] );
	  cout << "Run "<< argv[fileIdx] << endl;
	  Shell.cmd( "run",script,cout );
	}
    } 
  catch( ExceptionAbstract& e )
    {
      cout << "!! In file <" << argv[fileIdx] << "> : "  << e <<endl;
    }
  catch ( const char* str ) {
	  cout << "!! In file <" << argv[fileIdx] << "> : "
	  	  << "Unknown exception " << str << endl;
  }
  catch( ... ){ dgDEBUG(5) << "!! Unknown! " <<endl ; }
 
  while(1)
    {
      try
	{
	  dgDEBUG(5) << "Run shell." << endl;
	  Shell.shell(cin,cout);
	  dgDEBUG(5) << "Shell over." << endl;
      if( cin.eof() ) break;
	} 
      catch( ExceptionAbstract& e )
	{
	  cout << "!!  "  << e <<endl;
	}
      catch( ... ){ dgDEBUG(5) << "!! Unknown! " <<endl ; }
    }

  dgDEBUGOUT(15);
  return 0;
}




