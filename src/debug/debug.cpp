/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 */

#include <dynamic-graph/debug.h>
#include <fstream>
#include <ios>

using namespace dynamicgraph;

#ifdef WIN32
const char *DebugTrace::DEBUG_FILENAME_DEFAULT =
    "c:/tmp/dynamic-graph-traces.txt";
#else  /*WIN32*/
const char *DebugTrace::DEBUG_FILENAME_DEFAULT =
    "/tmp/dynamic-graph-traces.txt";
#endif /*WIN32*/

#ifdef VP_DEBUG
#ifdef WIN32
std::ofstream dg_debugfile("C:/tmp/dynamic-graph-traces.txt",
                           std::ios::trunc &std::ios::out);
#else  /*WIN32*/
std::ofstream dg_debugfile("/tmp/dynamic-graph-traces.txt",
                           std::ios::trunc &std::ios::out);
#endif /*WIN32*/
#else
std::ofstream dg_debugfile;
class dgDebug_init {
public:
  dgDebug_init() { dg_debugfile.setstate(std::ios::failbit); }
};
dgDebug_init dgDebug_initialisator;

#endif

namespace dynamicgraph {
DebugTrace dgDEBUGFLOW(dg_debugfile);
DebugTrace dgERRORFLOW(dg_debugfile);
} // namespace dynamicgraph

void DebugTrace::openFile(const char *filename) {
  if (dg_debugfile.good() && dg_debugfile.is_open())
    dg_debugfile.close();
  dg_debugfile.clear();
  dg_debugfile.open(filename, std::ios::trunc & std::ios::out);
}

void DebugTrace::closeFile(const char *) {
  if (dg_debugfile.good() && dg_debugfile.is_open()) {
    dg_debugfile.close();
  }
  dg_debugfile.setstate(std::ios::failbit);
}
