/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      ExceptionTraces.h
 * Project:   SOT
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


#ifndef __TRACES_EXCEPTION_H
#define __TRACES_EXCEPTION_H

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


#include <dynamic-graph/exception-abstract.h>
#include <dynamic-graph/dynamic-graph-api.h>

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph {

/* \class ExceptionTraces
 */
class DYNAMICGRAPH_EXPORT ExceptionTraces
:public ExceptionAbstract

{
 public:
  enum ErrorCodeEnum
    {
      GENERIC = ExceptionAbstract::TRACES

      ,NOT_OPEN
    };

  static const std::string EXCEPTION_NAME;
  virtual const std::string& getExceptionName( void ) const { return EXCEPTION_NAME; }

public:

  ExceptionTraces ( const ExceptionTraces::ErrorCodeEnum& errcode,
		       const std::string & msg = "" );
  ExceptionTraces( const ExceptionTraces::ErrorCodeEnum& errcode,
			const std::string & msg,const char* format, ... );
  virtual ~ExceptionTraces( void ) throw () {}


};



} //namespace dynamicgraph




#endif /* #ifndef __TRACES_EXCEPTION_H */

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
