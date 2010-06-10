/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      sotexception-signal.h
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


#ifndef __SIGNAL_EXCEPTION_H
#define __SIGNAL_EXCEPTION_H

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


#include <dynamic-graph/exception-abstract.h>
#include <dynamic-graph/dynamic-graph-API.h>

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph {

/* \class sotExceptionSignal
 */
class DYNAMICGRAPH_EXPORT ExceptionSignal
:public ExceptionAbstract

{
 public:
  enum ErrorCodeEnum
    {
      GENERIC = ExceptionAbstract::SIGNAL

      ,READWRITE_LOCK
      ,COPY_NOT_INITIALIZED
      ,NOT_INITIALIZED
      ,PLUG_IMPOSSIBLE
      ,SET_IMPOSSIBLE
      ,BAD_CAST
    };

  static const std::string EXCEPTION_NAME;
  virtual const std::string& getExceptionName( void ) const { return EXCEPTION_NAME; }

public:

  ExceptionSignal ( const ExceptionSignal::ErrorCodeEnum& errcode,
		       const std::string & msg = "" );
  ExceptionSignal( const ExceptionSignal::ErrorCodeEnum& errcode,
			const std::string & msg,const char* format, ... );
  virtual ~ExceptionSignal( void ){}


};

} // namespace dynamicgraph



#endif /* #ifndef __SIGNAL_EXCEPTION_H */

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
