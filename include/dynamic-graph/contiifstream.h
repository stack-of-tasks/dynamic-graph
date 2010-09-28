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
#  if defined (contiifstream_EXPORTS)
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




