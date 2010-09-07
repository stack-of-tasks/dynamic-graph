/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet Lagadic, 2005
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      dgDebug.h
 * Project:   STack of Tasks
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
 * Macro de trace et de debugage
 *
 *   - TRACAGE:    TRACE et ERROR_TRACE fonctionnent comme des printf
 * avec retour chariot en fin de fonction.
 *                 CERROR et CTRACE fonctionnent comme les flux de sortie
 * C++ cout et cerr.
 *   - DEBUGAGE:   DEBUG_TRACE(niv,  et DERROR_TRACE(niv, fonctionnent
 * comme des printf, n'imprimant que si le niveau de trace 'niv' est
 * superieur au mode de debugage VP_DEBUG_MODE.
 *                 CDEBUG(niv) fonctionne comme le flux de sortie C++ cout.
 *                 DEBUG_ENABLE(niv) vaut 1 ssi le niveau de tracage 'niv'
 * est superieur au  mode de debugage DEBUG_MODE. Il vaut 0 sinon.
 *   - PROG DEFENSIVE: DEFENSIF(a) vaut a ssi le mode defensif est active,
 * et vaut 0 sinon.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
   dgDebug_init( void )
     { dg_debugfile.setstate( std::ios::failbit ) ; /* dg_debugfile.close(); */ }
 };
 dgDebug_init dgDebug_initialisator;

#endif

namespace dynamicgraph {
	DebugTrace dgDEBUGFLOW(dg_debugfile);
	DebugTrace dgERRORFLOW(dg_debugfile);
}

void DebugTrace::openFile( const char * filename )
{
  if( dg_debugfile.good()&&dg_debugfile.is_open() ) dg_debugfile.close();
  dg_debugfile.clear();
  dg_debugfile.open( filename, std::ios::trunc&std::ios::out );
  //std::cout << filename << dg_debugfile.good() << dg_debugfile.is_open() << std::endl;
}

void DebugTrace::closeFile( const char * filename )
{
  if( dg_debugfile.good()&&dg_debugfile.is_open() ) { dg_debugfile.close(); }
  dg_debugfile.setstate( std::ios::failbit ) ;
}


//DebugTrace dgDebugFLOW(std::cout);
//DebugTrace dgERRORFLOW(std::cerr);

