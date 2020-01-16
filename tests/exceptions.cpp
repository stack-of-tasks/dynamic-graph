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

BOOST_AUTO_TEST_CASE(exception_abstract) {
  std::string msg_aea("Test exception abstract");
  ExceptionAbstract aEA(10, msg_aea);

  const char *aC = aEA.getMessage();
  output_test_stream output;
  output << aC;
  BOOST_CHECK(output.is_equal("Test exception abstract"));

  output << aEA;
  BOOST_CHECK(
      output.is_equal("AbstractError [#10]:  Test exception abstract\n"));

  std::string msg_aet("Test exception abstract");
  ExceptionTraces aET(ExceptionTraces::GENERIC, msg_aet);

  output << aET;
}
