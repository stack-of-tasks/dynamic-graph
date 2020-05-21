/**
\page subp_logger Loggers

\section sec_init_rt_logger Initialization of the logger

\subsection subsec_init_rt_logger_hcpp Header and preprocessor variable

In order to activate the logger you need to add the following lines:
\code
#define ENABLE_RT_LOG
#include <dynamic-graph/logger.h>
#include <dynamic-graph/real-time-logger.h>
\endcode

\subsection subsec_logger_ Initialize the output stream

It is possible to set the output stream of the messages inside a file:
\code
  dynamicgraph::RealTimeLogger::instance();
  of.open("/tmp/dg-LOGS.txt",std::ofstream::out|std::ofstream::app);
  dgADD_OSTREAM_TO_RTLOG (of);

  dynamicgraph::RealTimeLogger::destroy();
\endcode

\section sec_use_rt_logger Using the rt_logger


\code
// Somewhere in your library
dgRTLOG() << "your message. Prefer to use \n than std::endl."
\endcode


Here the output file is "/tmp/dg-LOGS.txt".


Specifying the file with __FILE__ and the line inside the file by __LINE__ are
necessary for the STREAM messages. Indeed they are indexed using the two values.
The default values "" and 0 for the counting are not well understood.
*/
