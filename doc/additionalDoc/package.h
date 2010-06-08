/**
\mainpage

\section intro_dynamicGraph Introduction

This package implements ...

@defgroup factory Plugin management modules.
@ingroup factory

This code implements the factory design pattern, making creation of features,
tasks and objects available.

Objects, which are derived from Entities, Tasks, or Features, can be
 declared within the code and compiled to shared librairies (.so/.dll files).
These librairies can be loaded at run-time using the sotPluginLoader methods,
and at the same time register their class names to the Factory (see the
sotFactory documentation to learn how).

The Factory can then create instances of these objects and subsequently
register them in the Pool, where they can be listed, accessed, and acted upon
(see sotPoolStorage documentation). Basic commands defined by entities include
signal connection graph file generation, help and name print, and signals.

Finally, a shell (command-line) interface is made available thanks to the
sotInterpretor class (see the file test_shell.cpp). Objects deriving from
Entity can expose their own commands by overriding the Entity's default
commandLine() method. It is possible to load a plugin to register custom
shell commands; see sotShellFunctions and sotShellProcedure for an example.

The public static objects (singletons) made available by including the
corresponding headers in this module are:
\li factory: FactoryStorage
\li pool: PoolStorage
\li Shell: Interpreter

\image html schema_plugin.png


@defgroup signals Signals
@ingroup softwaresupport
This part provides the mechanism to transfer information
from one feature to another.

*/
