/*
 * SignalCaster.cpp
 *
 *  Created on: Jun 14, 2010
 *      Author: blue
 */

#include <dynamic-graph/signal-caster.h>
#include <dynamic-graph/dynamic-graph-api.h>
#include <exception>
#include <boost/lambda/bind.hpp>
#include <string>
#include <sstream>
#include <algorithm>

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
	if ( existsCast(type) )
		throw ( 1 ); //TODO: throw "cast already registered for type" exception
	functions_[type.name()] = cast_functions_type(displayer,caster, tracer);
}

void SignalCaster::unregisterCast(const std::type_info& type) {
	size_t n = functions_.erase(type.name());
	if ( 0 == n ) // erase did not find element
		throw ( 1 ); // TODO: throw Cast not registered exception
}

bool SignalCaster::existsCast(const type_info& type) {
	return functions_.find(type.name()) != functions_.end();
}

void SignalCaster::disp(const any& object, ostream& os) {
	const char* type_name = object.type().name();
	map<string, cast_functions_type>::iterator it =
			functions_.find(type_name);
	if ( it == functions_.end() )
		throw 1;; //TODO: throw "cast not registered" exception
	(*it).second.get<0>()(object, os); // call display function (tuple index 0)
}

void SignalCaster::trace(const any& object, ostream& os) {
	const char* type_name = object.type().name();
	map<string, cast_functions_type>::iterator it =
			functions_.find(type_name);
	if ( it == functions_.end() )
		throw 1;; //TODO: throw "cast not registered" exception
	(*it).second.get<2>()(object, os); // call trace function (tuple index 2)
}

any SignalCaster::cast(const type_info& type, istringstream& iss) {
	const char* type_name = type.name();
	map<string, cast_functions_type>::iterator it =	functions_.find(type_name);
	if ( it == functions_.end() )
		throw 1;; //TODO: throw "cast not registered" exception
	return (*it).second.get<1>()(iss); // call cast function (tuple index 1)
}


/// The global instance of the caster class.
DYNAMICGRAPH_EXPORT SignalCaster g_caster;

SignalCastRegisterer::SignalCastRegisterer(const std::type_info& type, SignalCaster::displayer_type displayer,
		SignalCaster::caster_type caster, SignalCaster::tracer_type tracer)
{
	g_caster.registerCast(type, displayer, caster, tracer);
}

/// Default casts, such as casts already supported by std::iostream
template<typename T> class DefaultCastRegisterer : public SignalCastRegisterer {
public:
	DefaultCastRegisterer() : SignalCastRegisterer(typeid(T), disp, cast, trace) {}
	static boost::any cast(istringstream& iss) { T inst; iss >> inst; return inst; }
	static void disp(const any& object, ostream& os) { os << any_cast<T>(object) << endl;; }
	static void trace(const any& object, ostream& os) { disp(object,os); }
};

/// Registers useful casts
namespace {
	DefaultCastRegisterer<double> double_reg;
	DefaultCastRegisterer<int> int_reg;
	DefaultCastRegisterer<unsigned int> uint_reg;
}

} // namespace dynamicgraph

