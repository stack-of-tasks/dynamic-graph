/**
\page subp_signals Signals
\section sec_sigintro Signals

Entities can output different types of signals. To guarante real-time
perforamces, signals are implemented using C++ and mecanism which have a low
time foot-print. All signals are templated by a Time tick type parameter (which
is used in the caching of signals) - usually \c int. Signals are also templated
after the type of data they accept or provide. For example: (example) For a more
detailed programmer-oriented description of signals, please see \ref signals
*/
