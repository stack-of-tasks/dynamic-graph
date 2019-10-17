/**
\page tracerdoc Tracer
\section tracerdoc_description Description
The \b Tracer entity monitors a set of signals. With an input change on the
entity's [trigger] signal, the tracked signal values are recorded and traced to
a file. The behavior of the trace-to-file function can be changed, from printing
to a file immediately after recording, to printing out only when asked
explicitly.

\section tracerdoc_commands Commands
The \b commands that this entity exposes are:
\code
open, close (a file);
add (a signal)
clear (recorded values);
record (signal values)
trace (doesn't do anything)
start, stop (traces)
\endcode
\n
For more information on the signals exposed by this entity, please check the
code documentation of the dynamicgraph::Tracer class.
\n\n

\section tracerdoc_sample Sample usage
The following code creates a TracerRealTime entity and sets the tracing buffer
size to 80MB. It then tells the tracer to create files with names of the form:
/tmp/dg_XXX.dat where XXX is the signal name, and call the signal
after the device has evaluated the control law:
\code
robot.tracer = TracerRealTime("com_tracer")
robot.tracer.setBufferSize(80*(2**20))
robot.tracer.open('/tmp','dg_','.dat')
robot.device.after.addSignal('{0}.triger'.format(robot.tracer.name))
\endcode

\section tracerdoc_addi Additional information
See doxygen documentation for the class dynamicgraph::Tracer

\section tracerdoc_generates Generated plugin file
tracer.dll or tracer.so.
*/
