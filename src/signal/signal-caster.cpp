/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
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

#include <dynamic-graph/signal-caster.h>
#include <dynamic-graph/dynamic-graph-api.h>
#include <exception>
#include <boost/lambda/bind.hpp>
#include <string>
#include <sstream>
#include <algorithm>
#include <dynamic-graph/exception-signal.h>
#include "dynamic-graph/linear-algebra.h"
using namespace std;
using namespace boost;

namespace dynamicgraph {

SignalCaster::SignalCaster() {
	// nothing to initialize
}

SignalCaster::~SignalCaster() {
	// no special cleanup to do
}

void SignalCaster::registerCast(const type_info& type, SignalCaster::displayer_type displayer,
		SignalCaster::caster_type caster, SignalCaster::tracer_type tracer) {
  if ( existsCast(type) ) {
    // If type name has already been registered for same type, do not throw.
    if ( type_info_[type.name()] != &type) {
      std::string typeName(type.name());
      std::ostringstream os;
      os << "cast already registered for typename " << typeName << "\n"
	 << "and types differ: " << &type << " != " << type_info_[type.name()]
	 << ".\n"
	 << "A possible reason is that the dynamic library defining this type\n"
	 << "has been loaded several times, defining different symbols"
	 << " for the same type.";
      throw ExceptionSignal(ExceptionSignal::GENERIC,
			    os.str());
    }
  }
	functions_[type.name()] = cast_functions_type(displayer,caster, tracer);
	type_info_[type.name()] = &type;
}

void SignalCaster::unregisterCast(const std::type_info& type) {
	size_t n = functions_.erase(type.name());
	if ( 0 == n ) // erase did not find element
		throw ExceptionSignal(ExceptionSignal::GENERIC); // TODO: throw Cast not registered exception
}

bool SignalCaster::existsCast(const type_info& type) {
	return functions_.find(type.name()) != functions_.end();
}

void SignalCaster::disp(const any& object, ostream& os) {
	const char* type_name = object.type().name();
	map<string, cast_functions_type>::iterator it =
			functions_.find(type_name);

	if ( it == functions_.end() )
		throw ExceptionSignal(ExceptionSignal::BAD_CAST,
				      "unable to display type "+
				      std::string(type_name)+
				      " into ostream.");
	//TODO: throw "cast not registered" exception
	(*it).second.get<0>()(object, os); // call display function (tuple index 0)
}

void SignalCaster::trace(const any& object, ostream& os) {
	const char* type_name = object.type().name();
	map<string, cast_functions_type>::iterator it =
			functions_.find(type_name);
	if ( it == functions_.end() )
		throw ExceptionSignal(ExceptionSignal::BAD_CAST,
				      "unable to trace type "+
				      std::string(type_name)+
				      " into ostream.");
	//TODO: throw "cast not registered" exception
	(*it).second.get<2>()(object, os); // call trace function (tuple index 2)
}

any SignalCaster::cast(const type_info& type, istringstream& iss) {
	const char* type_name = type.name();
	map<string, cast_functions_type>::iterator it =	functions_.find(type_name);

	if ( it == functions_.end() )
	  {
	    throw ExceptionSignal(ExceptionSignal::BAD_CAST,
				  "unable to cast string into type "+
				  std::string(type_name)+".");
	  }
	//TODO: throw "cast not registered" exception
	return (*it).second.get<1>()(iss); // call cast function (tuple index 1)
}


/// The global instance of the caster class.
SignalCaster g_caster;

/// Registers useful casts
namespace {
	DefaultCastRegisterer<double> double_reg;
	DefaultCastRegisterer<int> int_reg;
	DefaultCastRegisterer<unsigned int> uint_reg;
  DefaultCastRegisterer<dynamicgraph::Vector> vectorCastRegisterer;
  DefaultCastRegisterer<dynamicgraph::Matrix> matrixCastRegisterer;
}

} // namespace dynamicgraph

