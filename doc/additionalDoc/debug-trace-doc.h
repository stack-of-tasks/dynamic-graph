/**
\page subp_dbg_trace Debugging with macros and level

\section subp_dbg_trace_intro Introduction
The idea of this class and collection of MACROS is to specify
a level for a message, and record the message in a stream according to this level.
In addition there are mechanisms allowing that no time
is wasted in condition test. You can therefore let the debugging messages
inside the code without impact on performances.

\section subp_dbg_trace_set_on_macros Setting up dgDEBUG macros

To allow message display the entity must be compiled with the macro
\code
#define VP_DEBUG 1
\endcode
Commenting or removing this macro disable all the messages specified
by the following macros.

The level up to which the messages are accepted for display is
specified by:
\code
#define VP_DEBUG_MODE 50
\endcode

In the constructor of the entity, the file where all the messages
are written is specified by:
\code
  dynamicgraph::dgDEBUGFLOW.openFile("/tmp/dynamic-graph-traces.txt");
\endcode

\section subp_dbg_trace_using_macros Using dgDEBUG macros

To print that the beginning of a method is being executed use the following macros:
\code
    dgDEBUGIN(5);
\endcode
5 here specifies the minimum level that you be specified by VP_DEBUG_MODE for this message
to be displayed.

It will generate the following output:
\code
/path_to/dynamic-graph/tests/debug-trace.cpp: testDebugTrace(#46) :# In {
\endcode
The output displays the name of the source file, the name of the method,
the line where is the macro, and the message itself.

When going out of the method:
\code
    dgDEBUGOUT(5);
\endcode

This generates the following output:
\code
/path_to/dynamic-graph/tests/debug-trace.cpp: testDebugTrace(#54) :# Out }
\endcode

A message inside the code is written through:
\code
      dgDEBUG(5) << "Here is a test" << std::endl;
\endcode

This generates the following output:
\code
/path_to/dynamic-graph/tests/debug-trace.cpp: testDebugTrace(#52) :Here is a test
\endcode

\section subp_dbg_trace_wrk_exp Working example

A full working example is given here:
\include ../tests/debug-trace.cpp
 */
