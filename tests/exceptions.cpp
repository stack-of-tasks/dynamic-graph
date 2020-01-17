// Copyright 2020 Olivier Stasse
// LAAS, CNRS

#include <dynamic-graph/exception-abstract.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/exception-traces.h>

#include <sstream>

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

using boost::test_tools::output_test_stream;
using namespace dynamicgraph;

BOOST_AUTO_TEST_CASE(exception_abstract_param) {

  /// Test param from Exception
  /// Default constructor
  ExceptionAbstract::Param aParamSimple;
  /// Advanced constructor
  ExceptionAbstract::Param aParam(60, "function_test", "my_file");
  aParamSimple.initCopy(aParam);
}

BOOST_AUTO_TEST_CASE(exception_abstract) {

  /// Test exception abstract with a simple message
  std::string msg_aea("Test exception abstract");
  ExceptionAbstract aEA(10, msg_aea);

  const char *aC = aEA.getMessage();
  output_test_stream output;
  output << aC;
  BOOST_CHECK(output.is_equal("Test exception abstract"));

  output << aEA;
  BOOST_CHECK(
      output.is_equal("AbstractError [#10]:  Test exception abstract\n"));
}

BOOST_AUTO_TEST_CASE(exception_traces) {

  std::string msg_aet("Test exception traces simple");
  ExceptionTraces aET(ExceptionTraces::GENERIC, msg_aet);
  output_test_stream output;
  output << aET;
  BOOST_CHECK(
      output.is_equal("TracesError [#300]:  Test exception traces simple\n"));

  /// Test exception traces with a format.
  int a = 2, b = 3;
  std::string msg_aet2("Test exception traces ");
  ExceptionTraces aET2(ExceptionTraces::GENERIC, msg_aet2, "(%d,%d)", a, b);
  output << aET2;
  BOOST_CHECK(
      output.is_equal("TracesError [#300]:  Test exception traces (2,3)\n"));
}
