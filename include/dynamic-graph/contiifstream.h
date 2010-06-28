/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      Contiifstream.h
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



#ifndef __CONTIIFSTREAM_HH__
#define __CONTIIFSTREAM_HH__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


#include <iostream>
#include <fstream>
#include <sstream>
#ifndef WIN32
#include <unistd.h>
#endif
#include <list>

#include <dynamic-graph/interpreter.h>
#ifndef WIN32
#include <pthread.h>
#endif
/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32)
#  if defined (sotContiifstream_EXPORTS)
#    define DYNAMICGRAPHCONTIIFSTREAM_EXPORT __declspec(dllexport)
#  else
#    define DYNAMICGRAPHCONTIIFSTREAM_EXPORT __declspec(dllimport)
#  endif
#else
#  define DYNAMICGRAPHCONTIIFSTREAM_EXPORT
#endif

namespace dynamicgraph {
  /* --------------------------------------------------------------------- */
  /* --- CLASS ----------------------------------------------------------- */
  /* --------------------------------------------------------------------- */
  class DYNAMICGRAPHCONTIIFSTREAM_EXPORT Contiifstream
  {
  protected:
    std::string filename;
    unsigned int cursor;
    static const unsigned int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    std::list< std::string > reader;
    bool first;

  public: /* --- Constructor --- */
    Contiifstream( const std::string& n="" );
    ~Contiifstream( void );
    void open( const std::string& n ) { filename=n; cursor=0; }

  public: /* --- READ FILE --- */
    bool loop( void );

  public: /* --- READ LIST --- */
    inline bool ready( void ) { return 0<reader.size();}
    std::string next( void ) ;


  };
} // namespace dynamicgraph

#endif /* #ifndef __CONTIIFSTREAM_HH__ */




