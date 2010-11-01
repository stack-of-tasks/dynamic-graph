/*
 * Copyright 2010,
 * François Bleibel,
 * Thomas Moulard,
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

\image html pictures/entity.png
Functionality:
\li Built-in scripting language* for fast prototyping and testing
\li Support for extensions and modules using dynamic link libraries
\li Template-based signal definition, independant
\li Type-safe connection of input and output signals
\li On-demand signal computation as well as a caching system for signal values allow fast
computation of signal values, which is a critical point for real-time systems\n
See \ref scriptingabout

\section overview Code overview

\section entities List of entities in this package
Since most of the functionality in projects using the dynamic-graph framework
is exposed from entities, here is a short description of all the entities contained in
this package. Note that most entities are contained in a binary file that closely matches
the entities' names in the scripts; loading this file with the plugin loader will
enable creation of this entity through the factory.
\li \ref tracerdoc
\li \ref tracerrealtimedoc
\li ShellProcedure
\li \ref shellfunctions_doc

The entities will be placed in ${PREFIX}/lib/plugin (since this may change, it is advised to
check the install log or the CMakeLists.txt file to check the installation path).
\section sigintro About signals

Entities can output different types of signals. All signals are templated by a Time
tick type parameter (which is used in the caching of signals) - usually \c int. Signals
are also templated after the type of data they accept or provide. For example:
(example)
For a more detailed programmer-oriented description of signals, please see \ref signals

\section scriptingabout Notes about the scripting language
The scripting language allows entities to define their own commands, and
provides a basic framework for working with the dynamic-graph.
At the time of writing, there is talk about replacing (or complementing) this limited
language with a python interpreter.

A couple of functions are built-in in the interpreter and provides low-level features such as file sourcing or
plug-in loading.\n
These functions are:\n
\code plug <obj1.sig1> <obj2.sig2> \endcode plugs the signal sig1 of obj1 to the signal sig2 of obj2. sig1 and sig2
have to be of the same type. sig1 has to be an output signal and sig2 an input signal.
\code new <class> <object> \endcode instantiates an object object of class class. object has to be a free identifier and
class an existing entity.
\code destroy <object> \endcode deletes an instance previously created.
\code run <script.txt> \endcode sources (i.e. read and interpret) an external file.
\code loadPlugin <file.so> <directory> \endcode loads a plugin called file.so and located in the directory directory.
\code unloadPlugin <path/file.so> \endcode unloads a plugin.
\code help \endcode lists available functions.
\code set <obj.signal> <value> \endcode defines an input signal to a specific, constant, value.
\code get <obj.signal> <value> \endcode prints out a signal value.
\code compute <obj.sig> <time> \endcode computes an output signal and sets the associated time to time.

\section usecase How to use this package
1) Programmatically\n
This code implements the factory design pattern, making creation of entities
available to packages depending on the dynamic-graph API.

Objects, which are derived from Entities (base class dynamicgraph::Entity), can be
 declared within the code and compiled to shared libraries (.so/.dll files).
These libraries can be loaded at run-time using the PluginLoader methods,
and at the same time register their class names to the Factory (see the
examples in the SOT documentation to learn how).

The Factory can then create instances of these objects and subsequently
register them in the Pool, where they can be listed, accessed, and acted upon
(see PoolStorage documentation). Basic commands defined by entities include
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
\li g_factory: dynamicgraph::FactoryStorage
\li g_pool: dynamicgraph::PoolStorage
\li g_shell: dynamicgraph::Interpreter

For an example of a program creating entities in C++, see the unit test
test_pool.cpp (in your package source directory/unitTesting).

2) Through scripts\n
The program \ref dgshell_doc can be used to have scripting access to the dynamic-graph
library, where you can execute scripts and commands, load plugins, create entities and connect signals.

Here is a typical use case for programmers:
\image html figures/use-case.svg

\section References
\anchor Mansard2007

<b>"Task sequencing for sensor-based control"</b>,
<em>N. Mansard, F. Chaumette,</em>
IEEE Trans. on Robotics, 23(1):60-72, February 2007


\defgroup dgraph Core classes and objects

\ingroup dgraph
@{

Classes, entities and binaries that make up the core of the dynamic-graph library are listed here.
@}

\defgroup signals Signals

\ingroup signals
@{
This part provides the mechanism to transfer information
from one entity to another. There are three main types of signals,
all deriving from the common class dynamicgraph::SignalBase :
\li dynamicgraph::Signal : a "normal" signal, passing data around \b by \b value
\li dynamicgraph::SignalPtr : a signal used for efficient passing of large data, by reference (or rather, C pointers)*
\li dynamicgraph::SignalTimeDependent : a signal that enforces a time dependency between other signals,
making sure its inputs are up to date on access, using a incrementing time tick as reference.

\n* Note: this may cause a problem if this package is used in a multithreaded program.

Signals can be grouped together using dynamicgraph::SignalArray.

Signals implement a caching mechanism by storing the last computation time tick.

Some signals can be plugged ("plug" script command) into one another or set through shell commands.

For more information, please see the individual signal pages.

\b Samples

\li The following code ensures the jacobian output signal uses up-to-date values for its
computations, when accessed.

\code
  // This signal returns the Jacobian of the current value
  //  according to the robot state
  dg::SignalTimeDependent<ml::Matrix,int> jacobianSOUT;

  (...)

  jacobianSOUT.addDependency( positionSIN );
  jacobianSOUT.addDependency( articularJacobianSIN );

\endcode

@}

\namespace dynamicgraph This is the namespace where every object and class of this library is located.
*/
