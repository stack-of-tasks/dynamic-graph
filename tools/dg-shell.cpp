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

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-JAPAN, Tsukuba, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      test_shell.cc
 * Project:   DYNAMIC-GRAPH
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
  g_shell.referencePluginLoader( &pl );

  int fileIdx;
  try
    {
      for( fileIdx=1;fileIdx<argc;++fileIdx )
	{
	  std::istringstream script( argv[fileIdx] );
	  cout << "Run "<< argv[fileIdx] << endl;
	  g_shell.cmd( "run",script,cout );
	}
    }
  catch( exception& e )
    {
      cout << "!! In file <" << argv[fileIdx] << "> : "  << e.what() <<endl;
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
	  g_shell.shell(cin,cout);
	  dgDEBUG(5) << "Shell over." << endl;
      if( cin.eof() ) break;
	}
      catch( exception& e )
	{
	  cout << "!!  "  << e.what() <<endl;
	}
      catch( ... ){ dgDEBUG(5) << "!! Unknown! " <<endl ; }
    }

  dgDEBUGOUT(15);
  return 0;
}




