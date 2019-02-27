/*
 * Copyright 2011, 2019 
 * LAAS-CNRS
 * Nicolas Mansard, François Bailly 
 *
 * This file is part of dynamic-graph.
 * dynamic-graph is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * dynamic-graph is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __dynamic_graph_signal_helper_H__
#define __dynamic_graph_signal_helper_H__

/* --- COMMON INCLUDE -------------------------------------------------- */

/* dg signals */
#include <dynamic-graph/entity.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/linear-algebra.h> 

/* --- MACROS ---------------------------------------------------------- */
#define SIGNAL_OUT_FUNCTION_NAME(name) name##SOUT_function

#define DECLARE_SIGNAL( name,IO,type )    ::dynamicgraph::Signal<type,int> m_##name##S##IO
#define CONSTRUCT_SIGNAL( name,IO,type )  m_##name##S##IO( getClassName()+"("+getName()+")::"+#IO+"put("+#type+")::"+#name )
#define BIND_SIGNAL_TO_FUNCTION(name,IO,type) m_##name##S##IO.setFunction(boost::bind(&EntityClassName::SIGNAL_OUT_FUNCTION_NAME(name),this,_1,_2));

/**/

#define DECLARE_SIGNAL_IN( name,type )    ::dynamicgraph::SignalPtr<type,int> m_##name##SIN
#define CONSTRUCT_SIGNAL_IN( name,type )  m_##name##SIN( NULL,getClassName()+"("+getName()+")::input("+#type+")::"+#name )

/**/

#define DECLARE_SIGNAL_OUT_FUNCTION(name,type) \
  type& SIGNAL_OUT_FUNCTION_NAME(name)(type&,int)

#define DEFINE_SIGNAL_OUT_FUNCTION(name,type) \
  type& EntityClassName::SIGNAL_OUT_FUNCTION_NAME(name)(type& s,int iter)

#define SIGNAL_OUT_FUNCTION( name )  name##SOUT_function

#define DECLARE_SIGNAL_OUT( name,type )                         \
  public:                                                       \
    ::dynamicgraph::SignalTimeDependent<type,int> m_##name##SOUT;	\
  protected:                                                    \
  type& SIGNAL_OUT_FUNCTION(name)( type&,int )

#define CONSTRUCT_SIGNAL_OUT( name,type,dep )		\
  m_##name##SOUT( boost::bind(&  EntityClassName::name##SOUT_function,this,_1,_2), \
	      dep,getClassName()+"("+getName()+")::output("+#type+")::"+#name )



/**************** INNER SIGNALS *******************/
#define SIGNAL_INNER_FUNCTION_NAME(name) name##SINNER_function

#define DECLARE_SIGNAL_INNER_FUNCTION(name,type) \
  type& SIGNAL_INNER_FUNCTION_NAME(name)(type&,int)

#define DEFINE_SIGNAL_INNER_FUNCTION(name,type) \
  type& EntityClassName::SIGNAL_INNER_FUNCTION_NAME(name)(type& s,int iter)

#define DECLARE_SIGNAL_INNER(name,type)                           \
  public:                                                       \
    ::dynamicgraph::SignalTimeDependent<type,int> m_##name##SINNER;\
  protected:                                                    \
    DECLARE_SIGNAL_INNER_FUNCTION(name,type)

#define CONSTRUCT_SIGNAL_INNER( name,type,dep )		        \
  m_##name##SINNER( boost::bind(&EntityClassName::name##SINNER_function,this,_1,_2), \
              dep,getClassName()+"("+getName()+")::inner("+#type+")::"+#name )


#endif // __dynamic_graph_signal_helper_H__
