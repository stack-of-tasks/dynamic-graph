/**
\page subp_installation Installation

\section sec_inst_dep Dependencies

dynamic-graph depends on:
<ul>
<li> boost </li>
<li> eigen </li>
<li> cmake </li>
</ul>

\section sec_inst_get_src Getting the source

The sources are available through github at the following URL:
<a
href="https://github.com/stack-of-tasks/dynamic-graph">https://github.com/stack-of-tasks/dynamic-graph</a>

To clone:
\code{.sh}
git clone https://github.com/stack-of-tasks/dynamic-graph.git
\endcode


\section sec_inst_comp Compiling

\code
cd dynamic-graph
mkdir _build
cd _build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE
make
\endcode


*/
