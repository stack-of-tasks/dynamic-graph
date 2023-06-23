/*
 * Copyright 2011, 2019
 * LAAS-CNRS
 * Nicolas Mansard, François Bailly
 *
 */

#ifndef __dynamic_graph_signal_helper_H__
#define __dynamic_graph_signal_helper_H__

/* --- COMMON INCLUDE -------------------------------------------------- */

/* dg signals */
#include <dynamic-graph/entity.h>
#include <dynamic-graph/linear-algebra.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>

/* --- MACROS ---------------------------------------------------------- */
#define SIGNAL_OUT_FUNCTION_NAME(name) name##SOUT_function

#define DECLARE_SIGNAL(name, IO, type) \
  ::dynamicgraph::Signal<type, sigtime_t> m_##name##S##IO
#define CONSTRUCT_SIGNAL(name, IO, type)                                    \
  m_##name##S##IO(getClassName() + "(" + getName() + ")::" + #IO + "put(" + \
                  #type + ")::" + #name)
#define BIND_SIGNAL_TO_FUNCTION(name, IO, type) \
  m_##name##S##IO.setFunction(boost::bind(      \
      &EntityClassName::SIGNAL_OUT_FUNCTION_NAME(name), this, _1, _2));

/**/

#define DECLARE_SIGNAL_IN(name, type) \
  ::dynamicgraph::SignalPtr<type, sigtime_t> m_##name##SIN
#define CONSTRUCT_SIGNAL_IN(name, type)                                        \
  m_##name##SIN(NULL, getClassName() + "(" + getName() + ")::input(" + #type + \
                          ")::" + #name)

/**/

#define DECLARE_SIGNAL_OUT_FUNCTION(name, type) \
  type &SIGNAL_OUT_FUNCTION_NAME(name)(type &, sigtime_t)

#define DEFINE_SIGNAL_OUT_FUNCTION(name, type)                    \
  type &EntityClassName::SIGNAL_OUT_FUNCTION_NAME(name)(type & s, \
                                                        sigtime_t iter)

#define SIGNAL_OUT_FUNCTION(name) name##SOUT_function

#define DECLARE_SIGNAL_OUT(name, type)                                 \
 public:                                                               \
  ::dynamicgraph::SignalTimeDependent<type, sigtime_t> m_##name##SOUT; \
                                                                       \
 protected:                                                            \
  type &SIGNAL_OUT_FUNCTION(name)(type &, sigtime_t)

#define CONSTRUCT_SIGNAL_OUT(name, type, dep)                                \
  m_##name##SOUT(                                                            \
      boost::bind(&EntityClassName::name##SOUT_function, this, _1, _2), dep, \
      getClassName() + "(" + getName() + ")::output(" + #type + ")::" + #name)

/**************** INNER SIGNALS *******************/
#define SIGNAL_INNER_FUNCTION_NAME(name) name##SINNER_function

#define DECLARE_SIGNAL_INNER_FUNCTION(name, type) \
  type &SIGNAL_INNER_FUNCTION_NAME(name)(type &, sigtime_t)

#define DEFINE_SIGNAL_INNER_FUNCTION(name, type)                    \
  type &EntityClassName::SIGNAL_INNER_FUNCTION_NAME(name)(type & s, \
                                                          sigtime_t iter)

#define DECLARE_SIGNAL_INNER(name, type)                                 \
 public:                                                                 \
  ::dynamicgraph::SignalTimeDependent<type, sigtime_t> m_##name##SINNER; \
                                                                         \
 protected:                                                              \
  DECLARE_SIGNAL_INNER_FUNCTION(name, type)

#define CONSTRUCT_SIGNAL_INNER(name, type, dep)                                \
  m_##name##SINNER(                                                            \
      boost::bind(&EntityClassName::name##SINNER_function, this, _1, _2), dep, \
      getClassName() + "(" + getName() + ")::inner(" + #type + ")::" + #name)

#endif  // __dynamic_graph_signal_helper_H__
