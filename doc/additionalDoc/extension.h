/**
\page subp_how_to_use Using this package

\section usecase How to use this package

\subsection use_programmatically General introduction

For control purposes the main use of this package is to create new entities and connect them through signals.

Objects, which are derived from Entities (base class dynamicgraph::Entity), can be
declared within the code and compiled as shared libraries (.so/.dll files).
These libraries can be loaded at run-time using the PluginLoader methods,
and at the same time register their class names to the Factory (see the
examples in the <a href="http://projects.laas.fr/gepetto/doc/stack-of-tasks/sot-core/master/doxygen-html">sot-core documentation</a> 
for advanced control examples).

The Factory can then create instances of these objects and subsequently
register them in the Pool. From the pool they can be listed, accessed, and acted upon
(see PoolStorage documentation). Basic commands defined by entities include
signal connection graph file generation, help and name print, and signals.
This is usually done through a scripting language such as python (see
<a hef="https://github.com/stack-of-tasks/dynamic-graph-python">dynamic-graph-python</a>)

The singletons made available by including the corresponding headers in this
module are:
\li dynamicgraph::FactoryStorage
\li dynamicgraph::PoolStorage

For an example of a program creating entities in C++, see the unit test
test_pool.cpp (in your package source directory/tests).

\subsection Tutorial
A tutorial is available <a href="http://stack-of-tasks.github.io/dynamic-graph-tutorial/">here</a>.
It is providing a step-by-step way of building an entity

\section sec_htw_helpers Helpers

When writing entities you might use some macros which are very useful to write your class.
They are given also in the <a href="http://projects.laas.fr/gepetto/doc/stack-of-tasks/sot-core/master/doxygen-html">sot-core</a> package as well.

\subsection subsec_howto_typedef Entity helpers

The header <b>entity-helper.h</b> is defining a type called EntityClassName 
\section sec_howto_macros_helpers Macro helpers

\subsection subsec_howto_macros_helpers Preprocessing macros for signals

<ul>
  <li>  Macro for input signals
    <ul>
      <li> <b>DECLARE_SIGNAL_IN(signal_name,type)</b>:
      Declare an input time dependent signal as a field of the class with the following name:
      \code
      m_signal_nameSIN
      \endcode
      </li>
      <li> <b>CONSTRUCT_SIGNAL_IN(signal_name,type)</b>:
      This macro is used in the constructor of the entity class handling this signal.
      It is calling the signal constructor and set the signal name to:
      \code
      EntityClassName(entity-name)::input(type)::signal_name
      \endcode
    </ul>
  </li>
  <li>  Macro for output signals
    <ul>
      <li> <b>DECLARE_SIGNAL_OUT(signal_name,type)</b>:
      Declare an output time dependent signal as a field of the class with the following name:
      \code
      m_signal_nameSOUT
      \endcode
      It also declares a method with the following pattern:
      \code
      type signal_nameSOUT_function(type &,int)
      \endcode
      The second pattern is the time when calling the output.
      </li>
      <li> <b>CONSTRUCT_SIGNAL_OUT(signal_name,type)</b>
      This macro is used in the constructor of the entity class handling this signal.
      It creates the binding to the method described previously, and set the signal name to:
      \code
      EntityClassName(entity_name)::output(type)::signal_name
      \endcode
      where entity_name is the name of the entity currently instanciated.
      </li>

      <li> <b>DEFINE_SIGNAL_OUT_FUNCTION(signal_name, type)</b>:
      This macro is used when implementing the method specific to the output signal.
      It is used in the main body of the entity class to declare the header of the method 
      with the following pattern:
      \code
      type EntityClassName::signal_nameSOUT_function(type &, int iter)
      \endcode
      </li>

    </ul>
  <li> 
  </li>
      <li> <b> DECLARE_SIGNAL_INNER(signal_name,type)</b>
      Inner signal are signal that depend on a state of the entity and not on input signals.
      This macro declares an inner signal with the following pattern:
      \code
      m_signal_nameSINNER
      \endcode
   </li>
   <li> <b>DEFINE_SIGNAL_INNER_FUNCTION</b> 
This macro 
</li>
</ul>

*/
