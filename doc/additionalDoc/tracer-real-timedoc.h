/**
\page tracerrealtimedoc TracerRealTime
\section description Description
The \b TracerRealTime entity monitors a set of signals with real-time constraints; its
function is very similar to the Tracer, except that all traces are recorded to a
memory buffer, which can be emptied at any time.
\section commands Commands
The \b commands that this entity exposes are (you can also type [entity name].help at the
dynamic-graph shell command line to see this list):
\code
empty (discards all buffers)
buffersize (sets buffer size for recording)
trace (writes traces to files)
\endcode
Plus all the commands exposed by \ref tracerdoc
\n
For more information on the signals exposed by this entity, please check the
code documentation of the dynamicgraph::Tracer class.

\section sample Sample usage
See \ref tracerdoc for a code sample of using TracerRealTime.

\section addi Additional information
See doxygen documentation for the class dynamicgraph::TracerRealTime

\section generates Generated plugin file
tracer-real-time.dll or tracer-real-time.so.

**/
