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

#include <dynamic-graph/contiifstream.h>
#include <dynamic-graph/debug.h>

namespace dynamicgraph
{
  Contiifstream::Contiifstream (const std::string& n)
    : filename (n),
      cursor (0),
      first (true)
  {}

  Contiifstream::~Contiifstream ()
  {
    dgDEBUGINOUT(5);
  }

  bool
  Contiifstream::loop ()
  {
    dgDEBUGIN(25);

    bool res = false;
    std::fstream file (filename.c_str ());
    file.seekg (cursor);
    file.sync ();

    while(1)
      {
	file.get (buffer,BUFFER_SIZE);
	if (file.gcount ())
	  {
	    res = true;
	    std::string line (buffer);
	    if (! first)
	      reader.push_back (line);
	    cursor=file.tellg ();
	    ++cursor;
	    file.get (*buffer); // get the last char ( = '\n')
	    dgDEBUG(15) << "line: "<< line<<std::endl;
	  }
	else
	  break;
      }

    first = false;
    dgDEBUGOUT(25);
    return res;
  }
  
  std::string
  Contiifstream::next ()
  {
    std::string res = *reader.begin ();
    reader.pop_front ();
    return res;
  }
} // end of namespace dynamicgraph.
