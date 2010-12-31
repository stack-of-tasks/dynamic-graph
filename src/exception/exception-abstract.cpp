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

#include <cstring>
#include <dynamic-graph/exception-abstract.h>
#include <dynamic-graph/debug.h>

namespace dynamicgraph {
  const std::string ExceptionAbstract::EXCEPTION_NAME = "Abstract";


  ExceptionAbstract::ExceptionAbstract (const int& _code,
					const std::string& _msg)
    : code (_code),
      message (_msg)
  {}

  const char*
  ExceptionAbstract::getMessage () const
  {
    return (this->message) .c_str ();
  }

  const std::string&
  ExceptionAbstract::getStringMessage () const
  {
    return this->message;
  }

  int
  ExceptionAbstract::getCode () const
  {
    return this->code;
  }

  ExceptionAbstract::Param&
  ExceptionAbstract::Param::initCopy (const Param& p)
  {
    if (&p == this)
      return *this;

    dgDEBUGIN(25);
    if (p.pointersSet)
      {
	strncpy (function,p.functionPTR, BUFFER_SIZE);
	strncpy (file,p.filePTR, BUFFER_SIZE);
	line = p.line;
	pointersSet = false;
	set = true;
      }
    else
      set = false;
    dgDEBUGOUT(25);
    return *this;
  }

  ExceptionAbstract::Param::Param (const int& _line,
				   const char* _function,
				   const char* _file)
    : functionPTR (_function),
      line (_line),
      filePTR (_file),
      pointersSet (true)
  {
    dgDEBUGINOUT(25);
  }

  std::ostream&
  operator << (std::ostream& os,
	       const ExceptionAbstract& error)
  {
    os << error.getExceptionName ()
       << "Error [#" << error.code << "]:  " << error.message << std::endl;

#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM
    if (error.p.set)
      os << "Thrown from " << error.p.file << ": " << error.p.function
	 <<" (#" << error.p.line << ")"<< std::endl;
#endif // DYNAMICGRAPH_EXCEPTION_PASSING_PARAM

    return os;
  }

} // end of namespace dynamicgraph.
