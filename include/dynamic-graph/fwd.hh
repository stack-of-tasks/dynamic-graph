// Copyright 2010, Thomas Moulard, JRL, CNRS/AIST
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// dynamic-graph. If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_FWD_HH
# define DYNAMIC_GRAPH_FWD_HH

namespace dynamicgraph
{
  class Contiifstream;
  class DebugTrace;

  class PluginRefMap;
  class Entity;
  class EntityRegisterer;
  class ExceptionAbstract;
  class ExceptionFactory;
  class ExceptionSignal;
  class ExceptionTraces;
  class FactoryStorage;
  class Interpreter;
  class InterpreterHelper;
  class OutStringStream;
  class PluginLoader;
  class PoolStorage;

  class SignalCaster;
  class SignalCastRegisterer;

  class Tracer;
  class TracerRealTime;

  template <typename T>
  class DefaultCastRegisterer;

  template <typename T, typename Time>
  class Signal;

  template <typename Time>
  class SignalArray;

  template <typename Time>
  class SignalArray_const;

  template <typename Time>
  class SignalBase;


  template <typename T, typename Time>
  class SignalPtr;
  template <typename T, typename Time>
  class SignalTimeDependent;
  template <typename Time>
  class TimeDependency;

  namespace command
  {
    class Command;
  } // end of namespace command.

} // end of namespace dynamicgraph.


#endif //! DYNAMIC_GRAPH_FWD_HH
