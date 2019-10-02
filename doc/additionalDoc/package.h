/*
 * Copyright 2010,
 * François Bleibel,
 * Thomas Moulard,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 */

/**
\mainpage

The dynamic-graph aims at building computational graphs for real-time control.
It provides the basic software functionnalities.

A more detailed introduction is available at \subpage subp_concept_intro.

The installation instruction are given at \subpage subp_installation.

The software graph structure is detailled in \subpage p_graph

For debugging your entities detailed instructions are given in \subpage debug

For citing the software in your research work please refer to
\subpage subp_references

\namespace dynamicgraph This is the namespace where every object and class of
this library is located.

\defgroup debug Debugging

\defgroup dgraph Core classes and objects
@{

Classes, entities and binaries that make up the core of the dynamic-graph
library are listed here.
@}

\defgroup signals Signals
@{
This part provides the mechanism to transfer information
from one entity to another. There are three main types of signals,
all deriving from the common class dynamicgraph::SignalBase :
\li dynamicgraph::Signal : a "normal" signal, passing data around \b by \b value
\li dynamicgraph::SignalPtr : a signal used for efficient passing of large data,
by reference (or rather, C pointers)* \li dynamicgraph::SignalTimeDependent : a
signal that enforces a time dependency between other signals, making sure its
inputs are up to date on access, using a incrementing time tick as reference.

\n* Note: this may cause a problem if this package is used in a multithreaded
program.

Signals can be grouped together using dynamicgraph::SignalArray.

Signals implement a caching mechanism by storing the last computation time tick.

For more information, please see the individual signal pages.

\b Samples

\li The following code ensures the jacobian output signal uses up-to-date values
for its computations, when accessed.

\code
  // This signal returns the Jacobian of the current value
  //  according to the robot state
  dg::SignalTimeDependent<ml::Matrix,int> jacobianSOUT;

  (...)

  jacobianSOUT.addDependency( positionSIN );
  jacobianSOUT.addDependency( articularJacobianSIN );

\endcode

@}

*/
