// Copyright 2010-2019, CNRS, JRL, AIST, LAAS
// Thomas Moulard, Olivier Stasse
//

#ifndef DYNAMIC_GRAPH_FWD_HH
#define DYNAMIC_GRAPH_FWD_HH

namespace dynamicgraph {
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
class Logger;
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

namespace command {
class Command;
}  // end of namespace command.

}  // end of namespace dynamicgraph.

#endif  //! DYNAMIC_GRAPH_FWD_HH
