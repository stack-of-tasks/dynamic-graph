//
// Created by corentin on 7/3/19.
//

#include <boost/foreach.hpp>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/signal-array.h>
#include <dynamic-graph/tracer.h>
#include <dynamic-graph/signal-cast-helper.h>

#include <assert.h>
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#define BOOST_TEST_MODULE signal_array

using boost::test_tools::output_test_stream;

dynamicgraph::SignalArray_const<double> sig;

using namespace std;
using namespace dynamicgraph;
using namespace dynamicgraph::command;

BOOST_AUTO_TEST_CASE(test_array) {
  SignalBase<int> sigBa("add test");
  SignalArray_const<int> sigArr_C(1);
  sigArr_C.operator<<(sigBa);
  sigArr_C.operator<<(sigBa);
  BOOST_CHECK_EQUAL(2, sigArr_C.getSize());

  SignalArray<int> sigArr(1);
  sigArr.operator<<(sigBa);
  sigArr.operator<<(sigBa);
  BOOST_CHECK_EQUAL(2, sigArr.getSize());

  SignalBase<int> sigB("constructor test");
  SignalArray<int> sigA(2);
  sigA << sigB;
  sigA.operator<<(sigB);
  SignalArray_const<int> sig_C(sigA);
  BOOST_CHECK_EQUAL(sigA.getSize(), sig_C.getSize());
}

BOOST_AUTO_TEST_CASE(test_base) {
  SignalBase<int> sigB("test");
  sigB.setReady();
  BOOST_CHECK_EQUAL(true, sigB.getReady());
}

BOOST_AUTO_TEST_CASE(test_cast_helper) {
  DefaultCastRegisterer<int> defaultCR;
  std::istringstream iss;
  iss.str("1");
  defaultCR.cast(iss);

  try {
    std::istringstream iss_fail;
    iss.str("test");
    defaultCR.cast(iss_fail);
  } catch (ExceptionSignal e) {
    // Take int, not string
  }
}