/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet Lagadic, 2005
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      dgDEBUG.h
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




#ifndef __DEBUG_HH
#define __DEBUG_HH

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include <dynamic-graph/dynamic-graph-API.h>

 
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

#ifndef VP_DEBUG_MODE
#define VP_DEBUG_MODE 0
#endif 
#ifndef VP_TEMPLATE_DEBUG_MODE
#define VP_TEMPLATE_DEBUG_MODE 0
#endif

#define DG_COMMON_TRACES do {  \
		    va_list arg; \
		    va_start(arg,format); \
		    vsnprintf( charbuffer,SIZE,format,arg ); \
		    va_end(arg); \
		    outputbuffer << tmpbuffer.str() << charbuffer <<std::endl; \
		} while(0)

namespace dynamicgraph {

class DYNAMICGRAPH_EXPORT DebugTrace
{
 public:
    static const int SIZE = 512;

    std::stringstream tmpbuffer;
    std::ostream& outputbuffer;
    char charbuffer[SIZE+1];
    int traceLevel;
    int traceLevelTemplate; 

    DebugTrace( std::ostream& os ): outputbuffer(os) {}

    inline void trace( const int level,const char* format,...)
	{ if( level<=traceLevel ) DG_COMMON_TRACES; tmpbuffer.str(""); }
    inline void trace( const char* format,...){ DG_COMMON_TRACES;  tmpbuffer.str(""); }
    inline void trace( const int level=-1 ) 
	{ if( level<=traceLevel ) outputbuffer << tmpbuffer.str(); tmpbuffer.str("");  }

    inline void traceTemplate( const int level,const char* format,...)
	{ if( level<=traceLevelTemplate ) DG_COMMON_TRACES; tmpbuffer.str(""); }
    inline void traceTemplate( const char* format,...)
	{ DG_COMMON_TRACES; tmpbuffer.str("");  }

    inline DebugTrace& pre( const std::ostream& dummy ) { return *this; }
    inline DebugTrace& pre( const std::ostream& dummy,int level )
	{ traceLevel = level; return *this; }
/*     inline DebugTrace& preTemplate( const std::ostream& dummy,int level )  */
/* 	{ traceLevelTemplate = level; return *this; } */


    static const char * DEBUG_FILENAME_DEFAULT;
    static void openFile( const char * filename = DEBUG_FILENAME_DEFAULT );
    static void closeFile( const char * filename = DEBUG_FILENAME_DEFAULT );

};

DYNAMICGRAPH_EXPORT extern DebugTrace dgDEBUGFLOW;
DYNAMICGRAPH_EXPORT extern DebugTrace dgERRORFLOW;

} // namespace dynamicgraph

#ifdef VP_DEBUG
#define dgPREDEBUG  __FILE__ << ": " <<__FUNCTION__  \
                              << "(#" << __LINE__ << ") :" 
#define dgPREERROR  "\t!! "<<__FILE__ << ": " <<__FUNCTION__  \
                            << "(#" << __LINE__ << ") :" 

#  define dgDEBUG(level) if( (level>VP_DEBUG_MODE)||(!dgDEBUGFLOW.outputbuffer.good()) ) ;\
    else dgDEBUGFLOW.outputbuffer << dgPREDEBUG
#  define dgDEBUGMUTE(level) if( (level>VP_DEBUG_MODE)||(!dgDEBUGFLOW.outputbuffer.good()) ) ;\
    else dgDEBUGFLOW.outputbuffer
#  define dgERROR  if(!dgDEBUGFLOW.outputbuffer.good()); else dgERRORFLOW.outputbuffer << dgPREERROR
#  define dgDEBUGF if(!dgDEBUGFLOW.outputbuffer.good()); else dgDEBUGFLOW.pre(dgDEBUGFLOW.tmpbuffer<<dgPREDEBUG,VP_DEBUG_MODE).trace
#  define dgERRORF if(!dgDEBUGFLOW.outputbuffer.good()); else dgERRORFLOW.pre(dgERRORFLOW.tmpbuffer<<dgPREERROR).trace
// TEMPLATE
#  define dgTDEBUG(level) if( (level>VP_TEMPLATE_DEBUG_MODE)||(!dgDEBUGFLOW.outputbuffer.good()) ) ;\
    else dgDEBUGFLOW.outputbuffer << dgPREDEBUG
#  define dgTDEBUGF  if(!dgDEBUGFLOW.outputbuffer.good()); else dgDEBUGFLOW.pre(dgDEBUGFLOW.tmpbuffer<<dgPREDEBUG,VP_TEMPLATE_DEBUG_MODE).trace
inline bool dgDEBUG_ENABLE( const int & level ) { return level<=VP_DEBUG_MODE; }
inline bool dgTDEBUG_ENABLE( const int & level ) { return level<=VP_TEMPLATE_DEBUG_MODE; }

/* -------------------------------------------------------------------------- */
#else // #ifdef VP_DEBUG
#define dgPREERROR  "\t!! "<<__FILE__ << ": " <<__FUNCTION__  \
                            << "(#" << __LINE__ << ") :" 
#  define dgDEBUG(level) if( 1 ) ; else std::cout
#  define dgDEBUGMUTE(level) if( 1 ) ; else std::cout
#  define dgERROR dgERRORFLOW.outputbuffer << dgPREERROR
inline void dgDEBUGF( const int level,const char* format,...) { return; }
inline void dgDEBUGF( const char* format,...) { return; }
inline void dgERRORF( const int level,const char* format,...) { return; }
inline void dgERRORF( const char* format,...) { return; }
// TEMPLATE
#  define dgTDEBUG(level) if( 1 ) ; else std::cout
inline void dgTDEBUGF( const int level,const char* format,...) { return; }
inline void dgTDEBUGF( const char* format,...) { return; }
#define dgDEBUG_ENABLE(level) false
#define dgTDEBUG_ENABLE(level) false

#endif // #ifdef VP_DEBUG
/* -------------------------------------------------------------------------- */

#define dgDEBUGIN(level) dgDEBUG(level) << "# In {" << std::endl
#define dgDEBUGOUT(level) dgDEBUG(level) << "# Out }" << std::endl
#define dgDEBUGINOUT(level) dgDEBUG(level) << "# In/Out { }" << std::endl

#define dgTDEBUGIN(level) dgTDEBUG(level) << "# In {" << std::endl
#define dgTDEBUGOUT(level) dgTDEBUG(level) << "# Out }" << std::endl
#define dgTDEBUGINOUT(level) dgTDEBUG(level) << "# In/Out { }" << std::endl


#endif /* #ifdef __VS_DEBUG_HH */

/*
 * Local variables:
 * c-basic-offset: 4
 * End:
 */
