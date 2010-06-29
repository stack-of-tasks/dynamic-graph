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



#ifndef __TRACER_H__
#define __TRACER_H__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* STD */
#include <string>
#include <list>
#include <boost/function.hpp>

/* DG */
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/time-dependency.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-traces.h>

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32) 
#  if defined (tracer_EXPORTS)
#    define DGTRACER_EXPORT __declspec(dllexport)
#  else  
#    define DGTRACER_EXPORT __declspec(dllimport)
#  endif 
#else
#  define DGTRACER_EXPORT
#endif

/* --------------------------------------------------------------------- */
/* --- TRACER ---------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph {

class DGTRACER_EXPORT Tracer
: public Entity
{
 protected:
  typedef std::list< const SignalBase<int>* > SignalList;
  SignalList toTraceSignals;


 public: 
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) { return CLASS_NAME; }

 public: 
  enum TraceStyle
    {
      WHEN_SAID   /// Record, then trace to file only when said to.
      ,EACH_TIME  /// Record and trace to file immediately.
      ,FREQUENTLY /// Record X time then trace (X is tuned by setFrenquence() ).
    };
  TraceStyle traceStyle;
  static const TraceStyle TRACE_STYLE_DEFAULT = EACH_TIME;
  double frequency;

  std::string basename;
  std::string suffix;
  std::string rootdir;
  bool namesSet;
  typedef std::list< std::ostream* > FileList;
  FileList files;
  typedef std::list< std::string > NameList;
  NameList names;
  bool play;
  int timeStart;

 public:
  Tracer( const std::string n );
  virtual ~Tracer( void ){ closeFiles(); }

  void addSignalToTrace( const SignalBase<int>& sig,
			 const std::string& filename="" );
  void clearSignalToTrace( void );
  //void parasite( SignalBase<int>& sig );
  void openFiles( const std::string& rootdir, const std::string& basename,
		  const std::string& suffix );
  virtual void closeFiles( void );

 protected:
  virtual void openFile( const SignalBase<int> & sig,
			 const std::string& filename );

 public:
  void setTraceStyle( const TraceStyle& style ){ traceStyle = style; }
  TraceStyle getTraceStyle( void ){ return traceStyle; }
  
  void setFrenquency( const double& frqu ){ frequency = frqu; }
  double getFrequency( void ){ return frequency; }

  void record( void );
  virtual void recordSignal( std::ostream& os,
			     const SignalBase<int>& sig );
  int& recordTrigger( int& dummy, const int& time );

  virtual void trace( void );

 public:
  //SignalTrigerer<int> triger;
  SignalTimeDependent<int,int> triger;

  /* --- DISPLAY ------------------------------------------------------------ */
  DGTRACER_EXPORT friend std::ostream& operator<< ( std::ostream& os,const Tracer& t );

  /* --- PARAMS --- */
  void display( std::ostream& os ) const;
  virtual void commandLine( const std::string& cmdLine
			    ,std::istringstream& cmdArgs
			    ,std::ostream& os );
};


} // namespace dynamicgraph


#endif /* #ifndef __TRACER_H__ */


