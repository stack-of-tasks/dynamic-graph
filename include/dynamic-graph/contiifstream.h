// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// dynamic-graph. If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_CONTIIFSTREAM_H
# define DYNAMIC_GRAPH_CONTIIFSTREAM_H
# include <fstream>
# include <iostream>
# include <list>
# include <sstream>

# ifndef WIN32
#  include <unistd.h>
#  include <pthread.h>
# endif //! WIN32

# include <dynamic-graph/fwd.hh>
# include <dynamic-graph/interpreter.h>
# include <dynamic-graph/config-contiifstream.hh>

namespace dynamicgraph
{
  class DG_CONTIIFSTREAM_DLLAPI Contiifstream
  {
  public:
    explicit Contiifstream (const std::string& n="");
    ~Contiifstream  ();

    inline void open (const std::string& n)
    {
      filename=n;
      cursor=0;
    }

    bool loop ();

    inline bool ready ()
    {
      return 0 < reader.size  ();
    }

    std::string next  ();

  protected:
    std::string filename;
    std::streamoff cursor;
    static const unsigned int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    std::list< std::string > reader;
    bool first;
  };
} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_CONTIIFSTREAM_H
