/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      Tracer.h
 * Project:   DG
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



#ifndef __TRACER_RT_H__
#define __TRACER_RT_H__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* STD */
#include <sstream>

/* DG */
#include <dynamic-graph/tracer.h>

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32) 
#  if defined (tracer_real_time_EXPORTS)
#    define DGTRACERREALTIME_EXPORT __declspec(dllexport)
#  else  
#    define DGTRACERREALTIME_EXPORT __declspec(dllimport)
#  endif 
#else
#  define DGTRACERREALTIME_EXPORT
#endif

/* --------------------------------------------------------------------- */
/* --- TRACER ---------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph {

class DGTRACERREALTIME_EXPORT OutStringStream
  : public std::ostringstream
{
public:
  char * buffer;
  unsigned int index;
  unsigned int bufferSize;
  bool full;
  std::string givenname;

public:

  OutStringStream( void );
  ~OutStringStream( void );    

  void resize( const unsigned int & size );
  bool addData( const char * data, const unsigned int & size );
  void dump( std::ostream& os );
  void empty( void );
  
};

class DGTRACERREALTIME_EXPORT TracerRealTime
: public Tracer
{
 public: 
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) { return CLASS_NAME; }

 protected:
  int bufferSize;
  static const int BUFFER_SIZE_DEFAULT = 1048576; //  1Mo
  typedef std::list< std::ofstream* > HardFileList;
  HardFileList hardFiles;

 public:
  TracerRealTime( const std::string n );
  virtual ~TracerRealTime( void ){ }

 public:
  virtual void closeFiles( void );
  virtual void trace( void );
 protected:
  virtual void openFile( const SignalBase<int> & sig,
			 const std::string& filename );
  virtual void recordSignal( std::ostream& os,
			     const SignalBase<int>& sig );
  void emptyBuffers( void );

 public:
  void setBufferSize( const int & SIZE ) { bufferSize=SIZE; }
  const int & getBufferSize( void ) { return bufferSize; }

 public:

  /* --- DISPLAY ------------------------------------------------------------ */
  void display( std::ostream& os ) const;
  DGTRACERREALTIME_EXPORT friend std::ostream& operator<< ( std::ostream& os,const TracerRealTime& t );

  /* --- PARAMS --- */
  virtual void commandLine( const std::string& cmdLine
			    ,std::istringstream& cmdArgs
			    ,std::ostream& os );
};


} // namespace dynamicgraph


#endif /* #ifndef __TRACER_RT_H__ */


