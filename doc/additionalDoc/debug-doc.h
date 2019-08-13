/**
\page debug Debugging

They are several ways to perform debugging in dynamic-graph depending on your needs or situation:
- Programmatically inside the entity in C++ will write inside a buffer in a different thread and output in a stream
(either std::cout or a file). It is detailed in \subpage subp_debug_rt_logger.
- Programmatically inside the entity in C++ using a member of the entities and the previous real-time mechanism.
It provides 4 levels of messags :(DEBUG,INFO, WARNING, ERROR). It is described in details here:
\subpage subp_logger
- Programmatically in C++ to avoid overhead with macros and handling level as an int: \subpage subp_dbg_trace
- If you just need to collect informations from signals (like rosbag). You can use
an entity called Tracer inside the graph:\subpage tracerdoc . <br>
A real time version exists
to write directly inside a memory buffer \subpage tracerrealtimedoc
**/
