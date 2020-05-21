/**
\page subp_debug_rt_logger Real-time Logger

 \section real_time_logger_quick_intro Quick introduction
 Each entity has a protected logger_ object.
 The simplest way to have information while running your graph is to initialize
 it in the constructor, and then display information in the methods.

 You can then change the level of information you want to display using
 either the entity method setLoggerVerbosityLevel()
 or the corresponding python bindings.

 \section real_time_logger_modifying_entities Putting information in your entity.

 It is possible to define the periodicity of the logger:
 \code
 logger_.setTimeSample(0.001); 
 \endcode

 To define the periodicity at which one wants to print information:
 \code
 logger_.setStreamPrintPeriod(0.005);
 \endcode

 Several level of verbosity are possible:
 \code
 logger_.setVerbosity(VERBOSITY_ALL);
 \endcode
 The full list of options are:
<ul>
<li>VERBOSITY_ALL: Accept all messages</li>
<li>VERBOSITY_INFO_WARNING_ERROR: Accept messages at minimum level : INFO,
WARNING, ERROR</li> <li>VERBOSITY_WARNING_ERROR: Accept messages at minimum
level : WARNING, ERROR</li> <li>VERBOSITY_ERROR: Accept messages at minimum
level : ERROR</li>
</ul>

It is specified by the enum LoggerVerbosity
 
 It is possible to display information according to various level of debug:
 \code
 sendMsg("This is a message of level MSG_TYPE_DEBUG", MSG_TYPE_DEBUG);
 \endcode
 or
 \code
 DYNAMIC_GRAPH_ENTITY_DEBUG   (*this) << "This is a message of level MSG_TYPE_DEBUG\n";
 DYNAMIC_GRAPH_ENTITY_INFO    (*this) << "This is a message of level MSG_TYPE_INFO\n";
 DYNAMIC_GRAPH_ENTITY_WARNING (*this) << "This is a message of level MSG_TYPE_WARNING\n";
 DYNAMIC_GRAPH_ENTITY_ERROR   (*this) << "This is a message of level MSG_TYPE_ERROR\n";
 DYNAMIC_GRAPH_ENTITY_DEBUG_STREAM   (*this) << "This is a message of level MSG_TYPE_DEBUG_STREAM\n";
 DYNAMIC_GRAPH_ENTITY_INFO_STREAM    (*this) << "This is a message of level MSG_TYPE_INFO_STREAM\n";
 DYNAMIC_GRAPH_ENTITY_WARNING_STREAM (*this) << "This is a message of level MSG_TYPE_WARNING_STREAM\n";
 DYNAMIC_GRAPH_ENTITY_ERROR_STREAM   (*this) << "This is a message of level MSG_TYPE_ERROR_STREAM\n";
 \endcode
 

   \note Thread safety. This class expects to have:
   - only one reader: the one who take the log entries and write them somewhere.
   - one writer at a time. Writing to the logs is **never** a blocking
     operation. If the resource is busy, the log entry is discarded.
*/
