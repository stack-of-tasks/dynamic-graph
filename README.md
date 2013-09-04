dynamic-graph
=============

[![Build Status](https://travis-ci.org/stack-of-tasks/dynamic-graph.png)](https://travis-ci.org/stack-of-tasks/dynamic-graph)
[![Coverage Status](https://coveralls.io/repos/stack-of-tasks/dynamic-graph/badge.png?branch=master)](https://coveralls.io/r/stack-of-tasks/dynamic-graph?branch=master)


This software provides an efficient way to modelize a C++ data-flow.

A dynamic graph data-flow is composed of:
 - entities (graph nodes)
 - signals (input/output of a graph action)

Output signals can then be plugged into input signals to data
transmission.

An efficient caching mechanism avoid useless data recomputation and a
simple built-in language can be used to control the graph actions.


Setup
-----

To compile this package, it is recommended to create a separate build
directory:

    mkdir _build
    cd _build
    cmake [OPTIONS] ..
    make install

Please note that CMake produces a `CMakeCache.txt` file which should
be deleted to reconfigure a package from scratch.
