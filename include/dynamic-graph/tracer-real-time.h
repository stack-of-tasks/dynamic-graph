// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_TRACER_REAL_TIME_H
#define DYNAMIC_GRAPH_TRACER_REAL_TIME_H
#include <sstream>

#include <dynamic-graph/config-tracer-real-time.hh>
#include <dynamic-graph/fwd.hh>
#include <dynamic-graph/tracer.h>

namespace dynamicgraph {
/// \ingroup plugin
///
/// \brief Stream for the tracer real-time.
class DG_TRACERREALTIME_DLLAPI OutStringStream : public std::ostringstream {
public:
  char *buffer;
  std::streamsize index;
  std::streamsize bufferSize;
  bool full;
  std::string givenname;

public:
  OutStringStream();
  ~OutStringStream();

  void resize(const std::streamsize &size);
  bool addData(const char *data, const std::streamoff &size);
  void dump(std::ostream &os);
  void empty();
};

/// \ingroup plugin
///
/// \brief Main class of the tracer real-time plug-in.
class DG_TRACERREALTIME_DLLAPI TracerRealTime : public Tracer {
  DYNAMIC_GRAPH_ENTITY_DECL();

public:
  TracerRealTime(const std::string &n);
  virtual ~TracerRealTime() {}

  virtual void closeFiles();
  virtual void trace();

  void display(std::ostream &os) const;
  DG_TRACERREALTIME_DLLAPI friend std::ostream &
  operator<<(std::ostream &os, const TracerRealTime &t);

  void emptyBuffers();

  void setBufferSize(const int &SIZE) { bufferSize = SIZE; }

  const int &getBufferSize() { return bufferSize; }

protected:
  virtual void openFile(const SignalBase<int> &sig,
                        const std::string &filename);

  virtual void recordSignal(std::ostream &os, const SignalBase<int> &sig);

  typedef std::list<std::ofstream *> HardFileList;
  static const int BUFFER_SIZE_DEFAULT = 1048576; //  1Mo

  int bufferSize;
  HardFileList hardFiles;
};
} // end of namespace dynamicgraph

#endif //! DYNAMIC_GRAPH_TRACER_REAL_TIME_H
