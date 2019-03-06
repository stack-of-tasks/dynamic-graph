/**
\page writegraphdoc Displaying the graph of entities
\section description Description
It is possible to view the graph of entities currently instanciated.
The format used by dynamic-graph is dot.
Using the python interpreter the following command
\code
from dynamic_graph import *
writeGraph('/tmp/my_dynamic_graph.dot')
\endcode
is writing the <b>my_dynamic_graph.dot</b> in the <b>/tmp</b> directory

\section fromdottopdf Viewing as a PDF file
To view the dot file you can simply use:
\code
dot -Tpdf /tmp/my_dynamic_graph.dot > /tmp/my_dynamic_graph.pdf
\endcode

It provides the following output:
\image html my_dynamic_graph.png

*/
