// Copyright 2011 Florent Lamiraux, Thomas Moulard.
//

#include <iostream>
#include "dynamic-graph/value.h"

#define BOOST_TEST_MODULE value

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

BOOST_AUTO_TEST_CASE(value_string) {
  using dynamicgraph::command::Value;

  std::string str1("value #1");
  Value value1(str1);
  Value value = value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=string, value=value #1"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=string, value=value #1"));
  }

  std::string str2("value #2");
  Value value2(str2);
  value = value2;

  {
    output_test_stream output;
    output << value2;
    BOOST_CHECK(output.is_equal("Type=string, value=value #2"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=string, value=value #2"));
  }
}
