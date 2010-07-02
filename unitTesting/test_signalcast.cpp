/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-JAPAN, Tsukuba, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      test_entity.cc
 * Project:   dynamicGraph
 * Author:    François Bleibel
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
	DefaultCastRegisterer<Vector> myVectorCast;
}

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
		myVectorSignal.setConstant(v);
		// print out signal value
		cout << "  ";
		myVectorSignal.get(cout);
		cout << endl;
	}


	return 0;
}
