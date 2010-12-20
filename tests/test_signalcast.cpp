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

/* -------------------------------------------------------------------------- */
/* --- INCLUDES ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


#include <string>
#include <iostream>
#include <cstdlib>
#include <memory>

#include <dynamic-graph/factory.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal.h>
// only included because we will define new casts here. Not needed in general.
#include <dynamic-graph/signal-caster.h>

#include "test_signalcast_libA.h"
#include "test_signalcast_libB.h"


using namespace std;
using namespace dynamicgraph;

// define a new cast with a ublas vector
#include <boost/numeric/ublas/vector.hpp>
// this header is *needed* to define iostream& operators << and >> for a ublas vector
#include <boost/numeric/ublas/io.hpp>
typedef boost::numeric::ublas::vector<double> Vector;
// define a new cast with a type that supports streaming operators to and from it
// (this could be automated with macros)
namespace {
	DefaultCastRegisterer<bool> myBooleanCast;
}

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
class BoostNumericsCastRegisterer
  : public SignalCastRegisterer
{
  typedef boost::numeric::ublas::vector<double> bnuVector;

  static boost::any castVector(std::istringstream& iss)
  {  bnuVector res; iss >> res; return res; }
  static void dispVector(const boost::any& object, std::ostream& os)
  {
    const bnuVector & v = boost::any_cast<bnuVector>(object);
    os << "[ ";
    for( unsigned int i=0;i<v.size();++i ) os << v(i) << " ";
    os << " ];" << std::endl;
  }
  static void traceVector(const boost::any& object, std::ostream& os)
  {
    const bnuVector & v = boost::any_cast<bnuVector>(object);
    for( unsigned int i=0;i<v.size();++i ) os << v(i) << " ";
    os << std::endl;
  }
public:
  BoostNumericsCastRegisterer(void)
    : SignalCastRegisterer(typeid(bnuVector),dispVector,castVector,traceVector) {}
};

BoostNumericsCastRegisterer myVectorCast;

int main() {
	using namespace boost::numeric::ublas;

	Signal<double, int> mySignal("out");
	istringstream value("42.0");
	cout << "[cast] Setting signal value to " << value.str() << endl;
	mySignal.set(value); // use "set" operation
	cout << "[disp] The value read is ";
	mySignal.get(cout);
	cout << "[trace] Printing out trace: ";
	mySignal.trace(cout);


	Signal<Vector, int> myVectorSignal("vector");
	// print out signal name
	cout << "  " << myVectorSignal << endl;

	cout << "[disp] Enumerating boost unit vectors" << endl;
	for (int i = 0; i < 5; ++ i) {
		unit_vector<double> v (5, i);
		std::ostringstream os;
		os << v;
		std::istringstream ss(os.str());
		myVectorSignal.set(ss);
		// print out signal value
		try{myVectorSignal.get(cout);}
		catch( const ExceptionAbstract & exp ) { cout << exp << std::endl; }
		cout << endl;
	}

	// check the following: "typeid of vA is different from typeid of vB
	// in different shared libraries""
	cout << "-- check typeid equality in shared libs" << endl;
	if(typeid(vA) == typeid(vB)) {
		cout << "The types of vA (libA.so) and vB (libB.so) are equal" << endl;
	} else {
		cout << "The types of vA (libA.so) and vB (libB.so) are different" << endl;
	}

	cout << "-- check type *name* equality in shared libs with type:" << endl
			<< "  " << typeid(vA).name() << endl;
	if( !strcmp(typeid(vA).name(), typeid(vB).name()) ) {
		cout << "The type names of vA (libA.so) and vB (libB.so) are equal" << endl;
	} else {
		cout << "The type names of vA (libA.so) and vB (libB.so) are different" << endl;
	}


	return 0;
}
