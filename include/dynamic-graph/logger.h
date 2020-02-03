/*
 * Copyright 2015, 2019
 * LAAS-CNRS
 * Andrea Del Prete, François Bailly, Olivier Stasse
 *
 */

#ifndef __dynamic_graph_logger_H__
#define __dynamic_graph_logger_H__

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined(WIN32)
#if defined(logger_EXPORTS)
#define LOGGER_EXPORT __declspec(dllexport)
#else
#define LOGGER_EXPORT __declspec(dllimport)
#endif
#else
#define LOGGER_EXPORT
#endif

namespace dynamicgraph {

/** Enum representing the different kind of messages.
 */
enum MsgType {
  MSG_TYPE_TYPE_BITS = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3, // 15
  MSG_TYPE_STREAM_BIT = 1 << 4,                           // 16

  MSG_TYPE_DEBUG = 1 << 3,                                          // 1
  MSG_TYPE_INFO = 1 << 2,                                           // 2
  MSG_TYPE_WARNING = 1 << 1,                                        // 4
  MSG_TYPE_ERROR = 1 << 0,                                          // 8
  MSG_TYPE_DEBUG_STREAM = MSG_TYPE_DEBUG | MSG_TYPE_STREAM_BIT,     // 17
  MSG_TYPE_INFO_STREAM = MSG_TYPE_INFO | MSG_TYPE_STREAM_BIT,       // 18
  MSG_TYPE_WARNING_STREAM = MSG_TYPE_WARNING | MSG_TYPE_STREAM_BIT, // 20
  MSG_TYPE_ERROR_STREAM = MSG_TYPE_ERROR | MSG_TYPE_STREAM_BIT      // 24
};
} // namespace dynamicgraph

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#include <map>
/// \todo These 3 headers should be removed.
#include <fstream>
#include <iomanip> // std::setprecision
#include <sstream>

#include <boost/assign.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <dynamic-graph/deprecated.hh>
#include <dynamic-graph/linear-algebra.h>
#include <dynamic-graph/real-time-logger-def.h>

namespace dynamicgraph {

//#define LOGGER_VERBOSITY_INFO_WARNING_ERROR
#define LOGGER_VERBOSITY_ALL

#define SEND_MSG(msg, type)                                                    \
  sendMsg(msg, type, __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__))

#define SEND_DEBUG_STREAM_MSG(msg) SEND_MSG(msg, MSG_TYPE_DEBUG_STREAM)
#define SEND_INFO_STREAM_MSG(msg) SEND_MSG(msg, MSG_TYPE_INFO_STREAM)
#define SEND_WARNING_STREAM_MSG(msg) SEND_MSG(msg, MSG_TYPE_WARNING_STREAM)
#define SEND_ERROR_STREAM_MSG(msg) SEND_MSG(msg, MSG_TYPE_ERROR_STREAM)

#define _DYNAMIC_GRAPH_ENTITY_MSG(entity, type)                                \
  (entity).logger().stream(type, __FILE__ BOOST_PP_STRINGIZE(__LINE__))

#define DYNAMIC_GRAPH_ENTITY_DEBUG(entity)                                     \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_DEBUG)
#define DYNAMIC_GRAPH_ENTITY_INFO(entity)                                      \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_INFO)
#define DYNAMIC_GRAPH_ENTITY_WARNING(entity)                                   \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_WARNING)
#define DYNAMIC_GRAPH_ENTITY_ERROR(entity)                                     \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_ERROR)

#define DYNAMIC_GRAPH_ENTITY_DEBUG_STREAM(entity)                              \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_DEBUG_STREAM)
#define DYNAMIC_GRAPH_ENTITY_INFO_STREAM(entity)                               \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_INFO_STREAM)
#define DYNAMIC_GRAPH_ENTITY_WARNING_STREAM(entity)                            \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_WARNING_STREAM)
#define DYNAMIC_GRAPH_ENTITY_ERROR_STREAM(entity)                              \
  _DYNAMIC_GRAPH_ENTITY_MSG(entity, MSG_TYPE_ERROR_STREAM)

template <typename T>
std::string toString(const T &v, const int precision = 3,
                     const int width = -1) {
  std::stringstream ss;
  if (width > precision)
    ss << std::fixed << std::setw(width) << std::setprecision(precision) << v;
  else
    ss << std::fixed << std::setprecision(precision) << v;
  return ss.str();
}

template <typename T>
std::string toString(const std::vector<T> &v, const int precision = 3,
                     const int width = -1, const std::string separator = ", ") {
  std::stringstream ss;
  if (width > precision) {
    for (unsigned int i = 0; i < v.size() - 1; i++)
      ss << std::fixed << std::setw(width) << std::setprecision(precision)
         << v[i] << separator;
    ss << std::fixed << std::setw(width) << std::setprecision(precision)
       << v[v.size() - 1];
  } else {
    for (unsigned int i = 0; i < v.size() - 1; i++)
      ss << std::fixed << std::setprecision(precision) << v[i] << separator;
    ss << std::fixed << std::setprecision(precision) << v[v.size() - 1];
  }

  return ss.str();
}

template <typename T>
std::string toString(const Eigen::MatrixBase<T> &v, const int precision = 3,
                     const int width = -1, const std::string separator = ", ") {
  std::stringstream ss;
  if (width > precision) {
    for (unsigned int i = 0; i < v.size() - 1; i++)
      ss << std::fixed << std::setw(width) << std::setprecision(precision)
         << v[i] << separator;
    ss << std::fixed << std::setw(width) << std::setprecision(precision)
       << v[v.size() - 1];
  } else {
    for (unsigned int i = 0; i < v.size() - 1; i++)
      ss << std::fixed << std::setprecision(precision) << v[i] << separator;
    ss << std::setprecision(precision) << v[v.size() - 1];
  }

  return ss.str();
}

enum LoggerVerbosity {
  VERBOSITY_ALL = MSG_TYPE_DEBUG,
  VERBOSITY_INFO_WARNING_ERROR = MSG_TYPE_INFO,
  VERBOSITY_WARNING_ERROR = MSG_TYPE_WARNING,
  VERBOSITY_ERROR = MSG_TYPE_ERROR,
  VERBOSITY_NONE = 0
};

/// \ingroup debug
///
/// \brief Class for logging messages
///
/// It is intended to be used like this:
/// \code
/// #define ENABLE_RT_LOG
/// #include <dynamic-graph/real-time-logger.h>
///
/// // Somewhere in the main function of your executable
/// int main (int argc, char** argv) {
///   std::ofstream of;
///   of.open("/tmp/dg-LOGS.txt",std::ofstream::out|std::ofstream::app);
///   dgADD_OSTREAM_TO_RTLOG (of);
/// }
///
/// // Somewhere in your library
/// dynamicgraph::LoggerVerbosity aLoggerVerbosityLevel =
///   VERBOSITY_WARNING_ERROR;
/// entity.setLoggerVerbosityLevel(aLoggerVerbosityLevel);
/// ...
/// // using macros
/// DYNAMIC_GRAPH_ENTITY_WARNING(entity) << "your message\n";
///
/// // or the equivalent code without macros:
/// // Please use '\n' instead of std::endl and flushing will have no effect
/// entity.logger.stream(dynamicgraph::MSG_TYPE_WARNING,
///                      __FILE__ BOOST_PP_STRINGIZE(__LINE__))
///   << your message << '\n';
///
/// \endcode
///
/// \todo remove m_timeSample and streamPrintPeriod to rather use a simple
///       integer counting the number of calls. This will achieve exactly the
///       same behaviour without rouding numerical errors.
class Logger {
public:
  /** Constructor */
  Logger(double timeSample = 0.001, double streamPrintPeriod = 1.0);

  /** Destructor */
  ~Logger();

  /** Method to be called at every control iteration
   * to decrement the internal Logger's counter. */
  void countdown();

  /** Get an output stream independently of the debug level.
   */
  RTLoggerStream stream() {
    return ::dynamicgraph::RealTimeLogger::instance().front();
  }

  /** Print the specified message on standard output if the verbosity level
   * allows it. The lineId is used to identify the point where sendMsg is
   * called so that streaming messages are printed only every streamPrintPeriod
   * iterations.
   * \param lineId typically __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__)
   */
  RTLoggerStream stream(MsgType type, const std::string &lineId = "") {
    RealTimeLogger &rtlogger = ::dynamicgraph::RealTimeLogger::instance();
    if (acceptMsg(type, lineId))
      return rtlogger.front();
    return rtlogger.emptyStream();
  }

  /** \deprecated instead, use
   *  \code
   *    stream(type, lineId) << msg << '\n';
   *  \endcode
   */
  void sendMsg(std::string msg, MsgType type, const std::string &lineId = "");

  /** \deprecated instead, use
   *  \code
   *    stream(type, lineId) << msg << '\n';
   *  \endcode
   */
  void sendMsg(std::string msg, MsgType type, const std::string &file,
               int line) DYNAMIC_GRAPH_DEPRECATED;

  /** Set the sampling time at which the method countdown()
   * is going to be called. */
  bool setTimeSample(double t);

  /** Get the sampling time at which the method countdown()
   * is going to be called. */
  double getTimeSample();

  /** Set the time period for printing of streaming messages. */
  bool setStreamPrintPeriod(double s);

  /** Get the time period for printing of streaming messages. */
  double getStreamPrintPeriod();

  /** Set the verbosity level of the logger. */
  void setVerbosity(LoggerVerbosity lv);

  /** Get the verbosity level of the logger. */
  LoggerVerbosity getVerbosity();

protected:
  LoggerVerbosity m_lv; /// verbosity of the logger
  double m_timeSample;
  /// specify the period of call of the countdown method
  double m_streamPrintPeriod; /// specify the time period of the stream prints
  double m_printCountdown;
  /// every time this is < 0 (i.e. every _streamPrintPeriod sec) print stuff

  typedef std::map<std::string, double> StreamCounterMap_t;
  /** Pointer to the dynamic structure which holds
      the collection of streaming messages */
  StreamCounterMap_t m_stream_msg_counters;

  inline bool isStreamMsg(MsgType m) { return (m & MSG_TYPE_STREAM_BIT); }

  /** Check whether a message of type \m and from \c lineId should be accepted.
   *  \note If \c is a stream type, the internal counter associated to \c lineId
   *        is updated.
   */
  bool acceptMsg(MsgType m, const std::string &lineId) {
    // If more verbose than the current verbosity level
    if ((m & MSG_TYPE_TYPE_BITS) > m_lv)
      return false;

    // if print is allowed by current verbosity level
    if (isStreamMsg(m))
      return checkStreamPeriod(lineId);
    return true;
  }

  /** Check whether a message from \c lineId should be accepted.
   *  \note The internal counter associated to \c lineId is updated.
   */
  bool checkStreamPeriod(const std::string &lineId);
};

} // namespace dynamicgraph

#endif // #ifndef __sot_torque_control_logger_H__
