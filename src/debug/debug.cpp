/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of dynamic-graph.
 * dynamic-graph is free software:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

