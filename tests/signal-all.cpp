//
// Created by corentin on 7/3/19.
//

#include <boost/foreach.hpp>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/signal-array.h>
#include <dynamic-graph/signal-cast-helper.h>
#include <dynamic-graph/tracer.h>

#include <assert.h>
#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

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

  res = false;
  try {
    sigB.setConstantDefault();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::PLUG_IMPOSSIBLE);
  }

  res = false;
  try {
    /// Check signal compatibility.
    sigB.checkCompatibility();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::PLUG_IMPOSSIBLE);
  }
  /// Verify set command value

  /// set
  std::istringstream iss("empty");
  res = false;
  try {
    sigB.set(iss);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }

  /// get a value
  res = false;
  std::ostringstream oss;
  try {
    sigB.get(oss);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }

  /// Trigger revaluation of the signal
  res = false;
  try {
    sigB.recompute(100);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }

  /// Trace the signal
  res = false;
  try {
    sigB.trace(oss);
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::SET_IMPOSSIBLE);
  }

  /// Display the signal
  sigB.display(output);
  BOOST_CHECK(output.is_equal(""));
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
