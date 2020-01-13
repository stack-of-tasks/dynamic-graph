// Copyright 2011 Florent Lamiraux, Thomas Moulard.
//

#include "dynamic-graph/value.h"
#include <iostream>

#define BOOST_TEST_MODULE value

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

BOOST_AUTO_TEST_CASE(value_bool) {
  using dynamicgraph::command::Value;

  bool abool1(false);
  Value value1(abool1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=bool, value=0"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=bool, value=0"));
  }
}

BOOST_AUTO_TEST_CASE(value_unsigned_int) {
  using dynamicgraph::command::Value;

  unsigned int aint1(5);
  Value value1(aint1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=unsigned int, value=5"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=unsigned int, value=5"));
  }
}

BOOST_AUTO_TEST_CASE(value_int) {
  using dynamicgraph::command::Value;

  int aint1(5);
  Value value1(aint1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=int, value=5"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=int, value=5"));
  }
}

BOOST_AUTO_TEST_CASE(value_float) {
  using dynamicgraph::command::Value;

  float afloat1(0.5);
  Value value1(afloat1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=float, value=0.5"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=float, value=0.5"));
  }
}

BOOST_AUTO_TEST_CASE(value_double) {
  using dynamicgraph::command::Value;

  double adouble1(0.5);
  Value value1(adouble1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=double, value=0.5"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=double, value=0.5"));
  }
}

BOOST_AUTO_TEST_CASE(value_vector) {
  using dynamicgraph::command::Value;

  dynamicgraph::Vector avector1;
  avector1.resize(2);
  avector1[0]=0.5;avector1[1]=1.5;
  Value value1(avector1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=vector, value=0.5\n1.5"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=vector, value=0.5\n1.5"));
  }
}

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

BOOST_AUTO_TEST_CASE(value_matrixXd) {
  using dynamicgraph::command::Value;

  Eigen::MatrixXd avector1;
  avector1.resize(2,2);
  avector1(0,0)=0.5;avector1(0,1)=1.5;
  avector1(1,0)=2.5;avector1(1,1)=3.5;
  Value value1(avector1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=matrixXd, value=0.5 1.5\n2.5 3.5"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=matrixXd, value=0.5 1.5\n2.5 3.5"));
  }
}

BOOST_AUTO_TEST_CASE(value_matrix4d) {
  using dynamicgraph::command::Value;

  Eigen::Matrix4d avector1;
  avector1.setZero();
  avector1(0,0)=0.5;avector1(0,1)=1.5;
  avector1(1,0)=2.5;avector1(1,1)=3.5;
  Value value1(avector1);
  Value value=value1;

  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=matrix4d, value=0.5 1.5   0   0\n"
                                "2.5 3.5   0   0\n  0   0   0   0\n"
                                "  0   0   0   0"));
  }

  {
    output_test_stream output;
    output << value;
    BOOST_CHECK(output.is_equal("Type=matrix4d, value=0.5 1.5   0   0\n"
                                "2.5 3.5   0   0\n  0   0   0   0\n"
                                "  0   0   0   0"));
  }
}
