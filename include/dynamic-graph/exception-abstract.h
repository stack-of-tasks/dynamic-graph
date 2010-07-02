/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      exception-abstract.h
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


#ifndef __ABSTRACT_EXCEPTION_H
#define __ABSTRACT_EXCEPTION_H

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


/* Classes standards. */
#include <iostream>                /* Classe ostream.    */
#include <string>                  /* Classe string.     */
#include <dynamic-graph/dynamic-graph-api.h>


// Uncomment this macros to have lines parameter on the throw display
// #define DYNAMIC-GRAPH_EXCEPTION_PASSING_PARAM 

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph {

/* \class ExceptionAbstract
 */
class DYNAMICGRAPH_EXPORT ExceptionAbstract : public std::exception
{

 public:

  enum ExceptionEnum
    {
      ABSTRACT = 0
      ,SIGNAL = 100
      ,FACTORY = 200
      ,TRACES = 300
    };

  static const std::string EXCEPTION_NAME;
  virtual const std::string& getExceptionName( void ) const { return EXCEPTION_NAME; }

 protected:
  /** Error code.
   * \sa ErrorCodeEnum */
  int code;

  /**  Error message (can be empty). */
  std::string message;

private:

  /**  forbid the empty constructor (private). */
  ExceptionAbstract( void );
public:

  ExceptionAbstract( const int& code, const std::string & msg = "" );
  virtual ~ExceptionAbstract( void ) throw() {}

  /**  Access to the error code. */
  int getCode (void);

  /** Reference access to the error message (can be empty). */
  const std::string& getStringMessage (void) const;

  /** Access to the pointer on the array of  \e char related to the error string.
   * Cannot be  \e NULL.
   */
  const char *getMessage (void);

  virtual const char* what() const throw()
  {
    return getStringMessage().c_str();
  }
  
  /** Print the error structure. */
  DYNAMICGRAPH_EXPORT friend std::ostream & operator << (std::ostream & os,
				     const ExceptionAbstract & err);

#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM 
 public:
  class Param
    {
    public:
      static const int BUFFER_SIZE = 80;

      const char * functionPTR;
      char function[ BUFFER_SIZE ];
      int line;
      const char * filePTR;
      char file[ BUFFER_SIZE ];
      bool pointersSet,set;
    public:
      Param( const int& _line, const char * _function, const char * _file );
      Param( void ) : pointersSet(false),set(false) {}
      Param& initCopy( const Param& p );
	
    };

 protected:
  mutable Param p;

  template<class Exc>
    friend const Exc& operator+ ( const ExceptionAbstract::Param& p, const Exc& e )
    { e.p.initCopy(p);   return e;    }
  template<class Exc>
    friend Exc& operator+ ( const ExceptionAbstract::Param& p, Exc& e )
    { e.p.initCopy(p);   return e;    }
#endif //#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM 
};

} // namespace dynamicgraph

#define DG_RETHROW ( const ExceptionAbstract& err ) { throw err; }



#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM
#  define DG_THROW throw ExceptionAbstract::Param(__LINE__,__FUNCTION__,__FILE__) +
#else //#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM 
#  define DG_THROW throw
#endif //#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM 


#endif /* #ifndef __ABSTRACT_EXCEPTION_H */

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
