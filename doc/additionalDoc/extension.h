/**
\page subp_how_to_use Using this package

\section usecase How to use this package

\subsection use_programmatically General introduction
Objects, which are derived from Entities (base class dynamicgraph::Entity), can be
declared within the code and compiled to shared libraries (.so/.dll files).
These libraries can be loaded at run-time using the PluginLoader methods,
and at the same time register their class names to the Factory (see the
examples in the <a href="http://projects.laas.fr/gepetto/doc/stack-of-tasks/sot-core/master/doxygen-html">sot-core documentation</a> 
for advanced control examples).

The Factory can then create instances of these objects and subsequently
register them in the Pool, where they can be listed, accessed, and acted upon
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
A tutorial is available <a href="http://stack-of-tasks.github.io/dynamic-graph-tutorial/">here</a>

\section sec_htw_helpers Helpers

When writing entities you might use some macros which are very useful to write your class.
They are given also in the <a href="http://projects.laas.fr/gepetto/doc/stack-of-tasks/sot-core/master/doxygen-html">sot-core</a> package as well.


*/
