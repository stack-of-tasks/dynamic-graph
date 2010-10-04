/**
\page tracerdoc Tracer
Generated plugin file tracer.dll or tracer.so.
\section description Description
The \b Tracer entity monitors a set of signals. With an input change on the entity's <trigger> signal,
the tracked signal values are recorded and traced to a file. The behavior of the trace-to-file
function can be changed, from printing to a file immediately after recording, to printing out
only when asked explicitly.\n
The \b commands that this entity exposes are (you can also type [entity name].help at the
dynamic-graph shell command line to see this list):
\code
open, close (a file);
add (a signal)
clear (recorded values);
record (signal values)
trace (recorded values to file)
start, stop (traces)
\endcode
\n
For more information on the signals exposed by this entity, please check the
code documentation of the dynamicgraph::Tracer class.
\n\n
\section sample Sample usage
The following code creates a TracerRealTime entity, then sets the tracing buffer
size to 10MB. It then tells the tracer to create files with names of the form:
jl_XXX.dat where XXX is the signal name, and adds a few signals after
clearing the traces;
\code
new TracerRealTime tr
tr.bufferSize 10485760

tr.open  ${TRACE_REPOSITORY} jl_ .dat
OpenHRP.periodicCall addSignal tr.triger
(...)
# --- TRACE ---
tr.clear
tr.add OpenHRP.forceRARM
tr.add dyn.0
tr.add jgain.gain
\endcode
**/
