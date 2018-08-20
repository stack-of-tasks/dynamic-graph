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
