/*
 * Copyright 2015, 2019
 * LAAS-CNRS
 * Andrea Del Prete, François Bailly, Olivier Stasse
 *
 */

#ifndef WIN32
#include <sys/time.h>
#else
#include <Windows.h>
#endif
#define ENABLE_RT_LOG

#include <dynamic-graph/logger.h>
#include <iomanip> // std::setprecision
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <dynamic-graph/real-time-logger.h>

namespace dynamicgraph {

Logger::Logger(double timeSample, double streamPrintPeriod)
    : m_timeSample(timeSample), m_streamPrintPeriod(streamPrintPeriod),
      m_printCountdown(0.0) {
  m_lv = VERBOSITY_ERROR;
}

Logger::~Logger() {}

void Logger::setVerbosity(LoggerVerbosity lv) { m_lv = lv; }

LoggerVerbosity Logger::getVerbosity() { return m_lv; }
void Logger::countdown() {
  if (m_printCountdown < 0.0)
    m_printCountdown = m_streamPrintPeriod;
  m_printCountdown -= m_timeSample;
}

void Logger::sendMsg(std::string msg, MsgType type, const std::string &lineId) {
  stream(type, lineId) << msg << '\n';
}

void Logger::sendMsg(std::string msg, MsgType type, const std::string &file,
                     int line) {
  std::ostringstream oss;
  oss << file << line;
  stream(type, oss.str()) << msg << '\n';
}

bool Logger::setTimeSample(double t) {
  if (t <= 0.0)
    return false;
  m_timeSample = t;
  return true;
}

bool Logger::setStreamPrintPeriod(double s) {
  if (s <= 0.0)
    return false;
  m_streamPrintPeriod = s;
  return true;
}

double Logger::getTimeSample() { return m_timeSample; }

double Logger::getStreamPrintPeriod() { return m_streamPrintPeriod; }

bool Logger::checkStreamPeriod(const std::string &lineId) {
  // insert element with value 0 if it does not exist.
  // otherwise, return a counter to the existing one.
  std::pair<StreamCounterMap_t::iterator, bool> result =
      m_stream_msg_counters.insert(std::make_pair(lineId, 0.));

  // if counter is greater than 0 then decrement it and do not print
  double &counter = result.first->second;
  if (counter > 0.0) {
    counter -= m_timeSample;
    return false;
  } else // otherwise reset counter and print
    counter = m_streamPrintPeriod;
  return true;
}

} // namespace dynamicgraph
