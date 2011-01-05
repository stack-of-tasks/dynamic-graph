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

#include <dynamic-graph/debug.h>
#include <fstream>
#include <ios>

using namespace dynamicgraph;

#ifdef WIN32
const char * DebugTrace::DEBUG_FILENAME_DEFAULT = "c:/tmp/dynamic-graph-traces.txt";
#else	/*WIN32*/
const char * DebugTrace::DEBUG_FILENAME_DEFAULT = "/tmp/dynamic-graph-traces.txt";
#endif	/*WIN32*/



#ifdef VP_DEBUG
 #ifdef WIN32
  std::ofstream dg_debugfile( "C:/tmp/dynamic-graph-traces.txt", std::ios::trunc&std::ios::out );
 #else	/*WIN32*/
  std::ofstream dg_debugfile( "/tmp/dynamic-graph-traces.txt", std::ios::trunc&std::ios::out );
#endif	/*WIN32*/
#else
std::ofstream dg_debugfile; //( "/dev/null", std::ios::trunc&std::ios::out );
 class dgDebug_init
 {
 public:
   dgDebug_init  ()
     { dg_debugfile.setstate( std::ios::failbit ) ; /* dg_debugfile.close (); */ }
 };
 dgDebug_init dgDebug_initialisator;

#endif

namespace dynamicgraph {
	DebugTrace dgDEBUGFLOW(dg_debugfile);
	DebugTrace dgERRORFLOW(dg_debugfile);
}

void DebugTrace::openFile( const char * filename )
{
  if( dg_debugfile.good ()&&dg_debugfile.is_open () ) dg_debugfile.close ();
  dg_debugfile.clear ();
  dg_debugfile.open( filename, std::ios::trunc&std::ios::out );
  //std::cout << filename << dg_debugfile.good () << dg_debugfile.is_open () << std::endl;
}

void DebugTrace::closeFile(const char *)
{
  if( dg_debugfile.good ()&&dg_debugfile.is_open () ) { dg_debugfile.close (); }
  dg_debugfile.setstate( std::ios::failbit ) ;
}


//DebugTrace dgDebugFLOW(std::cout);
//DebugTrace dgERRORFLOW(std::cerr);

