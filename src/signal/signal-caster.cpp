// -*- c++-mode -*-
// Copyright 2010 François Bleibel
// Thomas Moulard,
// Olivier Stasse
//

#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/signal-caster.h>
#include <exception>
#include <sstream>
#include <string>

#include <dynamic-graph/linear-algebra.h>

namespace dynamicgraph {

SignalCaster::SignalCaster() {}

SignalCaster::~SignalCaster() {}

void SignalCaster::destroy() {
  delete instance_;
  instance_ = 0;
}

void SignalCaster::registerCast(const std::type_info &type,
                                SignalCaster::displayer_type displayer,
                                SignalCaster::caster_type caster,
                                SignalCaster::tracer_type tracer) {
  if (existsCast(type)) {
    // If type name has already been registered for same type, do not throw.
    if (type_info_[type.name()] != &type) {
      std::string typeName(type.name());
      std::ostringstream os;
      os << "cast already registered for typename " << typeName << "\n"
         << "and types differ: " << &type << " != " << type_info_[type.name()]
         << ".\n"
         << "A possible reason is that the dynamic"
         << " library defining this type\n"
         << "has been loaded several times, defining different symbols"
         << " for the same type.";
      throw ExceptionSignal(ExceptionSignal::GENERIC, os.str());
    }
  }
  functions_[type.name()] = cast_functions_type(displayer, caster, tracer);
  type_info_[type.name()] = &type;
}

void SignalCaster::unregisterCast(const std::type_info &type) {
  size_t n = functions_.erase(type.name());
  if (0 == n) // erase did not find element
    // TODO: throw Cast not registered exception
    throw ExceptionSignal(ExceptionSignal::GENERIC);
}

bool SignalCaster::existsCast(const std::type_info &type) const {
  return functions_.find(type.name()) != functions_.end();
}

SignalCaster::cast_functions_type &
SignalCaster::getCast(const std::string &type_name) {
  std::map<std::string, cast_functions_type>::iterator it =
      functions_.find(type_name);

  if (it == functions_.end())
    // TODO: throw "cast not registered" exception
    throw ExceptionSignal(ExceptionSignal::BAD_CAST, "Cast not registered");
  return it->second;
}

void SignalCaster::disp(const boost::any &object, std::ostream &os) {
  getCast(object.type().name()).get<0>()(object, os);
}

void SignalCaster::trace(const boost::any &object, std::ostream &os) {
  getCast(object.type().name()).get<2>()(object, os);
}

std::vector<std::string> SignalCaster::listTypenames() const {
  std::vector<std::string> typeList;
  for (std::map<std::string, cast_functions_type>::const_iterator iter =
           functions_.begin();
       iter != functions_.end(); ++iter)
    typeList.push_back(iter->first);
  return typeList;
}

boost::any SignalCaster::cast(const std::type_info &type,
                              std::istringstream &iss) {
  return getCast(type.name()).get<1>()(iss);
}

/// Singleton on the library-wide instance of SignalCaster
SignalCaster *SignalCaster::getInstance(void) {
  if (instance_ == 0) {
    instance_ = new SignalCaster;
  }
  return instance_;
}

SignalCaster *SignalCaster::instance_ = 0;

} // namespace dynamicgraph
