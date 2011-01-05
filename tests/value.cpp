// Copyright 2011 Florent Lamiraux, Thomas Moulard.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// You should have received a copy of the GNU Lesser General Public License
// along with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "dynamic-graph/value.h"

#define BOOST_TEST_MODULE value

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

BOOST_AUTO_TEST_CASE (value_string)
{
  using dynamicgraph::command::Value;

  std::string str1("value #1");
  Value value1(str1);
  Value value = value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK (output.is_equal ("Type=string, value=value #1"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK (output.is_equal ("Type=string, value=value #1"));
  }

  std::string str2("value #2");
  Value value2(str2);
  value = value2;

  {
    output_test_stream output;
    output << value2;
    BOOST_CHECK (output.is_equal ("Type=string, value=value #2"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK (output.is_equal ("Type=string, value=value #2"));
  }
}
