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

#ifndef __EXCEPTION_FACTORY_H
#define __EXCEPTION_FACTORY_H

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


#include <dynamic-graph/exception-abstract.h>
#include <dynamic-graph/dynamic-graph-api.h>

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph {

/* \class sotExceptionFactory
 */
class DYNAMIC_GRAPH_DLLAPI ExceptionFactory
:public ExceptionAbstract

{
public:

  enum ErrorCodeEnum
    {
      GENERIC = ExceptionAbstract::FACTORY
      ,UNREFERED_OBJECT
      ,UNREFERED_SIGNAL
      ,UNREFERED_FUNCTION
      ,DYNAMIC_LOADING
      ,SIGNAL_CONFLICT
      ,FUNCTION_CONFLICT
      ,OBJECT_CONFLICT
      ,SYNTAX_ERROR    // j' aime bien FATAL_ERROR aussi faut que je la case qq part...
      ,READ_FILE
    };

  static const std::string EXCEPTION_NAME;
  virtual const std::string& getExceptionName( void )const{ return ExceptionFactory::EXCEPTION_NAME; }

  ExceptionFactory ( const ExceptionFactory::ErrorCodeEnum& errcode,
			const std::string & msg = "" );
  ExceptionFactory ( const ExceptionFactory::ErrorCodeEnum& errcode,
			const std::string & msg,const char* format, ... );
  virtual ~ExceptionFactory( void ) throw() {}

};


} // namespace dynamicgraph


#endif /* #ifndef __EXCEPTION_FACTORY_H */

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
