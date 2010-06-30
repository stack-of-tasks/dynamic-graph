/*
 *  Copyright
 */

#ifndef DYNAMICGRAPH_API_H
#define DYNAMICGRAPH_API_H

#if defined (WIN32)
#  ifdef dynamic_graph_EXPORTS
#    define DYNAMICGRAPH_EXPORT __declspec(dllexport)
#  else
#    define DYNAMICGRAPH_EXPORT __declspec(dllimport)
#  endif
#else
#  define DYNAMICGRAPH_EXPORT
#endif

#endif
