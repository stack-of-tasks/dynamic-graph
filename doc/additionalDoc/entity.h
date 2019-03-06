/**
\page subpage_entities Entities

\section section_entities Entities
\subsection entity_definition General definition
\image html entity.png

Despite the fact that it looks very similar to a ROS node or a CORBA/OpenRTM server, an entity is simply a C++ object.
The main idea is that this entity is providing mostly a data-driven functionnality working at very high rate (\f$ 200 Hz\f$ or \f$ 1 kHz \f$)
and should have a minimal computational time foot-print.

For this \subpage subp_signals (or ports to use a more classical terminology) are providing a time dependency between data.
To implement this, an output signal is linked with a method of the entity. The method calls input signals or use other means
to get the needed data.
It might be provided by the connection with remote computers through a middleware, or specific protocols,
but in general the external data is based upon the sensor values provided by a "Device" entity.
For this reason the signal evaluations are realized through the cascade of dependencies and start from the evaluation of an input
signal of a periodic node (in general the device). This is realized inside a \b real-time thread.

To add flexibility to a node, it is possible to add command with arguments to modify the internal behavior of the entity
or get information from the entity.
As a command is in general asynchronous and rare with respect to the data-flow scheme for the signals the command is in general
executed in a \b none-real-time thread.

\subsection entity_classes Entity class
Entities are implemented as C++ classes and compiled as dynamic libraries. They can be loaded and instancied dynamically.
It is therefore necessary to specify the location of their dynamical libraries.
However given the time it might take to load the library, it is not advised to do that during a control-law computation.
Entity instanciation also implies memory allocation which is also time consuming and thus not advised inside a real-time thread.

The entities will be placed in ${PREFIX}/lib/plugin (since this may change, it is advised to
check the install log or the CMakeLists.txt file to check the installation path).

\subsection entities List of entities in this package
Since most of the functionality in projects using the dynamic-graph framework
is exposed from entities, here is a short description of all the entities contained in
this package. Note that most entities are contained in a binary file that closely matches
the entities' names in the scripts; loading this file with the plugin loader will
enable creation of this entity through the factory.
\li \ref tracerdoc
\li \ref tracerrealtimedoc

\subsection specific_semantics Specific semantics with entities

It is possible to derive classes and apply specific semantic for the entities. In our case we are interested in specific control semantics:
\li Tasks (more information <a href="http://stack-of-tasks.github.io/sot-core/doxygen/HEAD/a00089.html">here</a>)
\li Features (more information <a href="http://stack-of-tasks.github.io/sot-core/doxygen/HEAD/a00030.html">here</a>)
\li Solver (more information <a href="http://stack-of-tasks.github.io/sot-core/doxygen/HEAD/a00078.html">here</a>)

*/
