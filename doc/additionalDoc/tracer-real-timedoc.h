/**
\page tracerrealtimedoc TracerRealTime

\section tracerrealtimedoc_description Description
The \b TracerRealTime entity monitors a set of signals with real-time
constraints; its function is very similar to the Tracer, except that all traces
are recorded to a memory buffer, which can be emptied at any time.

\section tracerrealtimedoc_commands Commands
The \b commands that this entity exposes are:
\code
empty (discards all buffers)
buffersize (sets buffer size for recording)
trace (writes traces to files)
\endcode
Plus all the commands exposed by \ref tracerdoc
\n
For more information on the signals exposed by this entity, please check the
code documentation of the dynamicgraph::Tracer class.

\section tracerrealtimedoc_sample Sample usage
See \ref tracerdoc for a code sample of using TracerRealTime.

\section tracerrealtimedoc_addi Additional information
See doxygen documentation for the class dynamicgraph::TracerRealTime

\section tracerrealtimedoc_generates Generated plugin file
tracer-real-time.dll or tracer-real-time.so.
*/
