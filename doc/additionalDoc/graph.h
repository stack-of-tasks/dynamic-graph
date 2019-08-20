/**
\page p_graph Graph


In this package, the graph considered are directed graphs.

In dynamic-graph a graph is build with:
- computational nodes which are entities \subpage subpage_entities.
- directed edges which are created by connecting input and output signals
\subpage subp_signals.
- managing the nodes is done through a factory \subpage subp_factory providing
classes and a way to create instances from this list of classes.
- the instances of node are handled through a pool \subpage subp_pool

We strongly recommend to use a scripting language such as Python to
manage the graph.
See <c>dynamic-graph-python</c> for more information on binding dynamic-graph
with Python.

It is possible to display the graph of entities \subpage writegraphdoc


*/
