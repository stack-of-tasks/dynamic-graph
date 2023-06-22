//
// Created by corentin on 7/3/19.
//

#include <assert.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/signal-array.h>
#include <dynamic-graph/signal-caster.h>
#include <dynamic-graph/tracer.h>

#include <boost/foreach.hpp>
#if BOOST_VERSION >= 105900
#include <boost/test/tools/output_test_stream.hpp>
#else
#include <boost/test/output_test_stream.hpp>
#endif
#include <boost/test/unit_test.hpp>
#include <iostream>

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
  SignalBase<int> sigA("testA");
  SignalBase<int> sigB("test");
  sigB.setReady();
  BOOST_CHECK_EQUAL(true, sigB.getReady());
  // Does nothing, just check that the interface
  // still exist at the abstract level.
  sigB.setPeriodTime(1);
  sigB.getPeriodTime();
  sigB.addDependency(sigA);
  sigB.removeDependency(sigA);
  sigB.clearDependencies();
  BOOST_CHECK_EQUAL(true, sigB.needUpdate(10));
  output_test_stream output;
  sigB.writeGraph(output);
  BOOST_CHECK(output.is_equal(""));

  /// Verify plug operations
  bool res = false;
  try {
    sigB.plug(&sigA);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::PLUG_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  res = false;
  try {
    sigB.unplug();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::PLUG_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  res = false;
  try {
    sigB.setConstantDefault();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::PLUG_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  /// Verify set command value

  /// set
  std::istringstream iss("empty");
  res = false;
  try {
    sigB.set(iss);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  /// get a value
  res = false;
  std::ostringstream oss;
  try {
    sigB.get(oss);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  /// Trigger revaluation of the signal
  res = false;
  try {
    sigB.recompute(100);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  /// Trace the signal
  res = false;
  try {
    sigB.trace(oss);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  /// Display the signal
  sigB.display(output);
  BOOST_CHECK(output.is_equal("Sig:test"));
}

BOOST_AUTO_TEST_CASE(test_cast_helper) {
  std::istringstream iss;
  iss.str("1");
  signal_io<int>::cast(iss);

  {
    std::istringstream iss_fail;
    iss.str("test");
    BOOST_CHECK_THROW(signal_io<int>::cast(iss_fail), ExceptionSignal);
  }

  /// Test cast register with Vector
  output_test_stream output;
  dynamicgraph::Vector avec;
  avec.resize(4);
  avec[0] = 1.0;
  avec[1] = 2.0;
  avec[2] = 3.0;
  avec[3] = 4.0;
  BOOST_CHECK_NO_THROW(signal_io<Vector>::trace(avec, output));

  /// Test cast register with Matrix
  dynamicgraph::Matrix amatrix;
  amatrix.resize(2, 2);
  amatrix(0, 0) = 0.0;
  amatrix(0, 1) = 1.0;
  amatrix(1, 0) = 2.0;
  amatrix(1, 1) = 3.0;
  BOOST_CHECK_NO_THROW(signal_io<Matrix>::trace(amatrix, output));

  std::istringstream aiss("test");
  signal_io<std::string>::cast(aiss);
}
