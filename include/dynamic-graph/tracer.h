// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_TRACER_H
#define DYNAMIC_GRAPH_TRACER_H
#include <boost/function.hpp>
#include <list>
#include <string>
#include <mutex>

#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-traces.h>
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/time-dependency.h>

#include <dynamic-graph/config-tracer.hh>

namespace dynamicgraph {
/// \ingroup plugin
///
/// \brief Tracer plug-in main class.
class DG_TRACER_DLLAPI Tracer : public Entity {
  DYNAMIC_GRAPH_ENTITY_DECL();

protected:
  typedef std::list<const SignalBase<int> *> SignalList;
  SignalList toTraceSignals;
  std::mutex files_mtx;

public:
  enum TraceStyle {
    WHEN_SAID
    /// Record, then trace to file only when said to.
    ,
    EACH_TIME
    /// Record and trace to file immediately.
    ,
    FREQUENTLY
    /// Record X time then trace (X is tuned by setFrenquence () ).
  };
  TraceStyle traceStyle;
  static const TraceStyle TRACE_STYLE_DEFAULT = EACH_TIME;
  double frequency;

  std::string basename;
  std::string suffix;
  std::string rootdir;
  bool namesSet;
  typedef std::list<std::ostream *> FileList;
  FileList files;
  typedef std::list<std::string> NameList;
  NameList names;
  bool play;
  int timeStart;

public:
  Tracer(const std::string n);
  virtual ~Tracer() { closeFiles(); }

  void addSignalToTrace(const SignalBase<int> &sig,
                        const std::string &filename = "");
  void addSignalToTraceByName(const std::string &signame,
                              const std::string &filename = "");
  void clearSignalToTrace();
  // void parasite( SignalBase<int>& sig );
  void openFiles(const std::string &rootdir, const std::string &basename,
                 const std::string &suffix);
  virtual void closeFiles();

protected:
  virtual void openFile(const SignalBase<int> &sig,
                        const std::string &filename);

public:
  void setTraceStyle(const TraceStyle &style) { traceStyle = style; }
  TraceStyle getTraceStyle() { return traceStyle; }

  void setFrenquency(const double &frqu) { frequency = frqu; }
  double getFrequency() { return frequency; }

  void record();
  virtual void recordSignal(std::ostream &os, const SignalBase<int> &sig);
  int &recordTrigger(int &dummy, const int &time);

  virtual void trace();
  void start() { play = true; }
  void stop() { play = false; }

public:
  // SignalTrigerer<int> triger;
  SignalTimeDependent<int, int> triger;

  /* --- DISPLAY --------------------------------------------------------- */
  DG_TRACER_DLLAPI friend std::ostream &operator<<(std::ostream &os,
                                                   const Tracer &t);

  /* --- PARAMS --- */
  void display(std::ostream &os) const;
};

} // end of namespace dynamicgraph

#endif //! DYNAMIC_GRAPH_TRACER_H
