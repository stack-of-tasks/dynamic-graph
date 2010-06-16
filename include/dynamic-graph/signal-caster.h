/*
 * SignalCaster.h
 *
 *  Created on: Jun 14, 2010
 *      Author: blue
 */

#ifndef SIGNALCASTER_H_
#define SIGNALCASTER_H_

#include <map>
#include <typeinfo>
#include <boost/function/function1.hpp>
#include <boost/function/function2.hpp>
#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>

namespace dynamicgraph {

/*! This class allows serialization of a number of objects into (disp) and from
 * (cast) std i/o streams. The transformation is done at run-time, i.e. SignalCaster
 * doesn't know about the type of objects it casts to. It also allows registering of
 * user-defined casts. A cast is identified by the compiler
 * The mapping from a type to a serialization function is dynamic, hence it is more
 * complex than a typical template-based compile-time resolve. So disp, cast and
 * trace are costly functions and should be used as such.
 */
class SignalCaster {
public:
	SignalCaster();
	virtual ~SignalCaster();

	/*! Typedef of displayer functions that take an encapsulated 'any' object
	 * and displays, cast, or trace it on an output stream (serialization).
	 */
	typedef boost::function2<void, const boost::any&, std::ostream&> displayer_type;
	typedef boost::function1<boost::any, std::istringstream&> caster_type;
	typedef boost::function2<void, const boost::any&, std::ostream&> tracer_type;

	/// Displays an object using a registered displayer function
	void disp(const boost::any& object, std::ostream& os);
	/// Traces an object using a registered trace function
	void trace(const boost::any& object, std::ostream& os);
	/// Casts an object using a registered cast function
	boost::any cast(const std::type_info&, std::istringstream& iss);
	/// Registers a cast
	void registerCast(const std::type_info& type, displayer_type displayer,
			caster_type caster, tracer_type tracer);
	/// Unregisters a cast
	void unregisterCast(const std::type_info& type);
	/// Checks if there is a displayer registered with type_name
	bool existsCast(const std::type_info& type);
private:
	/// Container for the three cast functions
	typedef boost::tuple<displayer_type, caster_type, tracer_type> cast_functions_type;
	/*! This map associates the typename of objects and the corresponding
	 * using boost::function with 'compatible' syntax
	 */
	std::map<std::string, cast_functions_type> functions_;
};


/// The library-wide instance of SignalCaster
extern SignalCaster g_caster;

/*
 * The SignalCast registerer class. Can be used to automatically register a cast when
 * instanced somewhere in a cpp file. Pass the typeid() of the type you want to
 * register a cast to as the first argument.
 * The code is provided here so the class does not need to be exported.
 */
class SignalCastRegisterer {
public:
	SignalCastRegisterer(const std::type_info& type, SignalCaster::displayer_type displayer,
			SignalCaster::caster_type caster, SignalCaster::tracer_type tracer) {
		g_caster.registerCast(type, displayer, caster, tracer);
	}
};

/*!
 * Global signal cast template (helper) functions
 *
 * Using these avoid using the typeid() operator and keeps the implementation
 * details hidden.
 */
template<typename T> void signal_disp(const T& value, std::ostream& os)
	{ g_caster.disp(value, os); }

template<typename T> T signal_cast(std::istringstream& iss)
	{ return boost::any_cast<T>(g_caster.cast(typeid(T), iss)); }

template<typename T> void signal_trace(const T& value, std::ostream& os)
	{ g_caster.trace(value, os); }

} // namespace dynamicgraph

#endif /* SIGNALCASTER_H_ */
