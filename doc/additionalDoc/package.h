/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of dynamic-graph.
 * dynamic-graph is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * dynamic-graph is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
\mainpage

\section intro_dynamicGraph Introduction

The dynamic-graph package is used to connect computation nodes, "entities"
together using a graph system, akin to what Simulink does. With the building
blocks this package provides, you can easily create a full computation graph
for a given problem. It is the basis for the stack of tasks operation.

\section overview Code overview
This code implements the factory design pattern, making creation of entities
available to packages depending on the dynamic-graph API.

Objects, which are derived from Entities, can be
 declared within the code and compiled to shared libraries (.so/.dll files).
These libraries can be loaded at run-time using the PluginLoader methods,
and at the same time register their class names to the Factory (see the
examples in the SOT documentation to learn how).

The Factory can then create instances of these objects and subsequently
register them in the Pool, where they can be listed, accessed, and acted upon
(see sotPoolStorage documentation). Basic commands defined by entities include
signal connection graph file generation, help and name print, and signals.

Finally, a shell (command-line) interface is made available thanks to the
Interpreter class (see the file test_shell.cpp for an example). Objects deriving from
Entity can expose their own commands by overriding the Entity's default
commandLine() method. It is possible to load a plugin to register custom
shell commands; see shell-functions and shell-procedure for an example.

Some basic shell functions, and support for procedures, are also included.
For a complete list of those, load the plugin shell-functions.so and type 'help'
at the command line.

The public static objects (singletons) made available by including the
corresponding headers in this module are:
\li g_factory: FactoryStorage
\li g_pool: PoolStorage
\li g_shell: Interpreter

\image html pictures/use-case.svg

\section entities List of entities in this package
Since most of the functionality in projects using the dynamic-graph framework
is exposed from entities, here is a short description of all the entities contained in
this package:
\li Tracer (generated plugin file tracer.dll or tracer.so)
The Tracer entity monitors a set of signals. With a change of the entity's <trigger> signal,
the tracked signal values are recorded and traced to a file. The behavior of the trace-to-file
function can be changed, from printing to a file immediately after recording, to printing out
only when asked explicitly.\br
The commands that this entity exposes are (you can also type [entity name].help at the
dynamic-graph shell command line to see this list):\br
open, close, add (a signal), clear, record, trace, start, stop
\br\br
For more information on the signals exposed by this entity, please check the
code documentation of the dynamicgraph::Tracer class.

\li TracerRealTime
\li ShellProcedure
\li ShellFunctions
\li Contiifstream

The entities will be placed in ${PREFIX}/lib/plugin (since this may change, it is advised to
check the install log or the CMakeLists.txt file to check the installation path).

\defgroup dgraph Core classes and objects
\defgroup signals Signals

\ingroup dgraph
@{

Classes that make up the core of the dynamic-graph library are listed here.
@}
\ingroup signals
@{
This part provides the mechanism to transfer information
from one feature to another. There are three main types of signals,
all deriving from the common class dynamicgraph::SignalBase :
\li dynamicgraph::Signal
\li dynamicgraph::SignalPtr
\li dynamicgraph::SignalTimeDependent

Signals can be grouped together using dynamicgraph::SignalArray.

Signals implement a caching mechanism by storing the last computation time tick.
Signals can be plug into one another or set through shell commands.
@}

\namespace dynamicgraph This is the namespace where every object and class of this library is located.
*/
