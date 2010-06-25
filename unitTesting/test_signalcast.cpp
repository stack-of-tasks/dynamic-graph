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

#include <dynamic-graph/factory.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal.h>

#include <memory>

using namespace std;
using namespace dynamicgraph;

int main() {
	Signal<double, int> mySignal("out");
	istringstream value("42.0");
	cout << "[cast] Setting signal value to " << value.str() << endl;
	mySignal.set(value); // use "set" operation
	cout << "[disp] The value read is ";
	mySignal.get(cout);
	cout << "[trace] Printing out trace: ";
	mySignal.trace(cout);

	return 0;
}
