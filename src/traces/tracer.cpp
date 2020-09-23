/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 */

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* DG */
#include <boost/bind.hpp>
#include <dynamic-graph/all-commands.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/tracer.h>
#include <dynamic-graph/value.h>

using namespace std;
using namespace dynamicgraph;
using namespace dynamicgraph::command;

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Tracer, "Tracer");

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

Tracer::Tracer(const std::string n)
    : Entity(n), toTraceSignals(), traceStyle(TRACE_STYLE_DEFAULT),
      frequency(1), basename(), suffix(".dat"), rootdir(), namesSet(false),
      files(), names(), play(false), timeStart(0),
      triger(boost::bind(&Tracer::recordTrigger, this, _1, _2), sotNOSIGNAL,
             "Tracer(" + n + ")::triger") {
  signalRegistration(triger);

  /* --- Commands --- */
  {
    using namespace dynamicgraph::command;
    std::string doc;

    doc = docCommandVoid2("Add a new signal to trace.", "string (signal name)",
                          "string (filename, empty for default");
    addCommand("add",
               makeCommandVoid2(*this, &Tracer::addSignalToTraceByName, doc));

    doc =
        docCommandVoid0("Remove all signals. If necessary, close open files.");
    addCommand("clear",
               makeCommandVoid0(*this, &Tracer::clearSignalToTrace, doc));

    doc = docCommandVoid3(
        "Gives the args for file opening, and "
        "if signals have been set, open the corresponding files.",
        "string (dirname)", "string (prefix)", "string (suffix)");
    addCommand("open", makeCommandVoid3(*this, &Tracer::openFiles, doc));

    doc = docCommandVoid0("Close all the open files.");
    addCommand("close", makeCommandVoid0(*this, &Tracer::closeFiles, doc));

    doc = docCommandVoid0("If necessary, dump "
                          "(can be done automatically for some traces type).");
    addCommand("dump", makeCommandVoid0(*this, &Tracer::trace, doc));

    doc = docCommandVoid0("Start the tracing process.");
    addCommand("start", makeCommandVoid0(*this, &Tracer::start, doc));

    doc = docCommandVoid0("Stop temporarily the tracing process.");
    addCommand("stop", makeCommandVoid0(*this, &Tracer::stop, doc));

    addCommand("getTimeStart",
               makeDirectGetter(*this, &timeStart,
                                docDirectGetter("timeStart", "int")));
    addCommand("setTimeStart",
               makeDirectSetter(*this, &timeStart,
                                docDirectSetter("timeStart", "int")));
  } // using namespace command
}

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

void Tracer::addSignalToTrace(const SignalBase<int> &sig,
                              const string &filename) {
  dgDEBUGIN(15);
  // openFile may throw so it should be called first.
  if (namesSet)
    openFile(sig, filename);
  toTraceSignals.push_back(&sig);
  dgDEBUGF(15, "%p", &sig);
  names.push_back(filename);
  triger.addDependency(sig);
  dgDEBUGOUT(15);
}

void Tracer::addSignalToTraceByName(const string &signame,
                                    const string &filename) {
  dgDEBUGIN(15);
  istringstream iss(signame);
  SignalBase<int> &sig = PoolStorage::getInstance()->getSignal(iss);
  addSignalToTrace(sig, filename);
  dgDEBUGOUT(15);
}

/*! Empty the list of signals to trace. This function
 * does not modify the file list (it does not close
 * the files in particular.
 */
void Tracer::clearSignalToTrace() {
  closeFiles();
  toTraceSignals.clear();
  triger.clearDependencies();
}

// void Tracer::
// parasite( SignalBase<int>& sig )
// {
//   triger.parasite(sig);
// }

void Tracer::openFiles(const std::string &rootdir_,
                       const std::string &basename_,
                       const std::string &suffix_) {
  dgDEBUGIN(15);
  std::basic_string<char>::size_type n = rootdir_.length();
  rootdir = rootdir_;
  if ((0 < n) & ('/' != rootdir[n - 1]))
    rootdir += '/';

  basename = basename_;
  suffix = suffix_;

  if (files.size())
    closeFiles();

  SignalList::const_iterator iter = toTraceSignals.begin();
  NameList::const_iterator iterName = names.begin();
  while (toTraceSignals.end() != iter) {
    dgDEBUG(15) << "Open <" << (*iter)->getName() << "> in <" << *iterName
                << ">." << std::endl;
    openFile(**iter, *iterName);
    ++iter;
    ++iterName;
  }

  namesSet = true;
  dgDEBUGOUT(15);
}

void Tracer::openFile(const SignalBase<int> &sig, const string &givenname) {
  dgDEBUGIN(15);
  string signame;
  if (givenname.length()) {
    signame = givenname;
  } else {
    signame = sig.shortName();
  }

  string filename = rootdir + basename + signame + suffix;

  dgDEBUG(5) << "Sig <" << sig.getName() << ">: new file " << filename << endl;
  std::ofstream *newfile = new std::ofstream(filename.c_str());
  files.push_back(newfile);
  dgDEBUGOUT(15);
}

void Tracer::closeFiles() {
  dgDEBUGIN(15);
  std::lock_guard<std::mutex> files_lock(files_mtx);

  for (FileList::iterator iter = files.begin(); files.end() != iter; ++iter) {
    std::ostream *filePtr = *iter;
    delete filePtr;
  }
  files.clear();

  dgDEBUGOUT(15);
}

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

void Tracer::record() {
  if (!play) {
    dgDEBUGINOUT(15);
    return;
  }

  dgDEBUGIN(15);

  // Ensure record() never hangs. If the attempt to acquire the lock fails,
  // then closeFiles() is active and we shouldn't write to files anyways.
  std::unique_lock<std::mutex> files_lock(files_mtx, std::try_to_lock);
  if (!files_lock.owns_lock()) {
    dgDEBUGOUT(15);
    return;
  }

  if (files.size() != toTraceSignals.size()) {
    DG_THROW
    ExceptionTraces(ExceptionTraces::NOT_OPEN, "No files open for tracing",
                    " (file=%d != %d=sig).", files.size(),
                    toTraceSignals.size());
  }

  FileList::iterator iterFile = files.begin();
  SignalList::iterator iterSig = toTraceSignals.begin();

  while (toTraceSignals.end() != iterSig) {
    dgDEBUG(45) << "Try..." << endl;
    recordSignal(**iterFile, **iterSig);
    ++iterSig;
    ++iterFile;
  }
  dgDEBUGOUT(15);
}

void Tracer::recordSignal(std::ostream &os, const SignalBase<int> &sig) {
  dgDEBUGIN(15);

  try {
    if (sig.getTime() > timeStart) {
      os << sig.getTime() << "\t";
      sig.trace(os);
      os << endl;
    }
  } catch (ExceptionAbstract &exc) {
    os << exc << std::endl;
  } catch (...) {
    os << "Unknown error occurred while reading signal." << std::endl;
  }

  dgDEBUGOUT(15);
}

int &Tracer::recordTrigger(int &dummy, const int &time) {
  dgDEBUGIN(15) << "    time=" << time << endl;
  record();
  dgDEBUGOUT(15);
  return dummy;
}

void Tracer::trace() {}

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

void Tracer::display(std::ostream &os) const {
  os << CLASS_NAME << " " << name << " [mode=" << (play ? "play" : "pause")
     << "] : " << endl
     << "  - Dep list: " << endl;
  for (SignalList::const_iterator iter = toTraceSignals.begin();
       toTraceSignals.end() != iter; ++iter) {
    os << "     -> " << (*iter)->getName() << endl;
  }
}

std::ostream &operator<<(std::ostream &os, const Tracer &t) {
  t.display(os);
  return os;
}
