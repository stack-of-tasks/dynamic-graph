// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software:
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
