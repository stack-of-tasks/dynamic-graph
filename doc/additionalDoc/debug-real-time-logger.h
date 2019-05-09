/**  
\page subp_debug_rt_logger Real-time Logger

 It is intended to be used like this:
   \code
   #define ENABLE_RT_LOG
   #include <dynamic-graph/real-time-logger.h>
  
   // Somewhere in the main function of your executable
   int main (int argc, char** argv) {
     dgADD_OSTREAM_TO_RTLOG (std::cout);
   }
  
   // Somewhere in your library
   dgRTLOG() << "your message. Prefer to use \n than std::endl."
   \endcode
  
   \note Thread safety. This class expects to have:
   - only one reader: the one who take the log entries and write them somewhere.
   - one writer at a time. Writing to the logs is **never** a blocking
     operation. If the resource is busy, the log entry is discarded.
*/
