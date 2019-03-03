/** 
\page subp_logger Loggers

\section sec_logger Initialization of the logger

\subsection subsec_logger_hcpp Header and preprocessor variable

In order to activate the logger you need to add the following lines:
\code
#define ENABLE_RT_LOG
#include <dynamic-graph/real-time-logger.h>
#include <dynamic-graph/logger.h>
\endcode

\subsection subsec_logger_ Initialize the output stream

It is possible to set the output stream of the messages inside a file:
\code
  dynamicgraph::RealTimeLogger::instance();
  of.open("/tmp/dg-LOGS.txt",std::ofstream::out|std::ofstream::app); 
  dgADD_OSTREAM_TO_RTLOG (of);
  
  dynamicgraph::RealTimeLogger::destroy();
\endcode
Here the file is "/tmp/dg-LOGS.txt". 

\subsection subsec_logger_init Initialization of the logger

Inside the constructor of the entity:
\code
      logger_.setTimeSample(0.001);
      logger_.setStreamPrintPeriod(0.005);
      logger_.setVerbosity(VERBOSITY_ALL);
      LoggerVerbosity alv = logger_.getVerbosity();
\endcode

The first line sets the frequency at which the logger will be updated.<br>
The second line specifies at which frequency the message should be
printed.<br>
The third line specifies the level of message to accept.<br>
The fourth line returns the level of verbosity.
In this case, all messages are accepted. <br>

The full list of options are:
<ul>
<li>VERBOSITY_ALL: Accept all messages</li>
<li>VERBOSITY_INFO_WARNING_ERROR: Accept messages at minimum level : INFO, WARNING, ERROR</li>
<li>VERBOSITY_WARNING_ERROR: Accept messages at minimum level : WARNING, ERROR</li>
<li>VERBOSITY_ERROR: Accept messages at minimum level : ERROR</li>
</ul>


\section sec_logger_tests Displaying messages

Here is some example on how to display or record some information.
\code
      sendMsg("This is a message of level MSG_TYPE_DEBUG",MSG_TYPE_DEBUG);
      sendMsg("This is a message of level MSG_TYPE_INFO",MSG_TYPE_INFO);
      sendMsg("This is a message of level MSG_TYPE_WARNING",MSG_TYPE_WARNING);
      sendMsg("This is a message of level MSG_TYPE_ERROR",MSG_TYPE_ERROR);
      sendMsg("This is a message of level MSG_TYPE_DEBUG_STREAM",MSG_TYPE_DEBUG_STREAM);
      sendMsg("This is a message of level MSG_TYPE_INFO_STREAM",MSG_TYPE_INFO_STREAM);
      sendMsg("This is a message of level MSG_TYPE_WARNING_STREAM",MSG_TYPE_WARNING_STREAM);
      sendMsg("This is a message of level MSG_TYPE_ERROR_STREAM",MSG_TYPE_ERROR_STREAM);

      logger_.countdown();
\endcode
*/
