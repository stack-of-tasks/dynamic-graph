/*
 * Copyright 2011, Nicolas Mansard, LAAS-CNRS
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

/* --- MACROS ---------------------------------------------------------- */

#define DECLARE_SIGNAL( name,IO,type )    ::dynamicgraph::Signal<type,int> name##S##IO
#define CONSTRUCT_SIGNAL( name,IO,type )  name##S##IO( getClassName()+"("+getName()+")::"+#IO+"put("+#type+")::"+#name )

#define DECLARE_SIGNAL_IN( name,type )    ::dynamicgraph::SignalPtr<type,int> name##SIN
#define CONSTRUCT_SIGNAL_IN( name,type )  name##SIN( NULL,getClassName()+"("+getName()+")::input("+#type+")::"+#name )

#define SIGNAL_OUT_FUNCTION( name )  name##SOUT_function
#define DECLARE_SIGNAL_OUT( name,type )                         \
  public:                                                       \
    ::dynamicgraph::SignalTimeDependent<type,int> name##SOUT;	\
  protected:                                                    \
  type& SIGNAL_OUT_FUNCTION(name)( type&,int )
#define CONSTRUCT_SIGNAL_OUT( name,type,dep )		\
  name##SOUT( boost::bind(&  EntityClassName::name##SOUT_function,this,_1,_2), \
	      dep,getClassName()+"("+getName()+")::output("+#type+")::"+#name )




#endif // __dynamic_graph_signal_helper_H__
