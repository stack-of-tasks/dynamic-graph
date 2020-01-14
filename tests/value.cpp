// Copyright 2011 Florent Lamiraux, Thomas Moulard.
//

#include <dynamic-graph/exception-factory.h>
#include "dynamic-graph/value.h"
#include <iostream>

#define BOOST_TEST_MODULE value

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

namespace dg = dynamicgraph;


BOOST_AUTO_TEST_CASE(value_none) {
  using dg::command::Value;

  Value value1;
  Value value(value1);
  {
    output_test_stream output;
    output << value1;
    BOOST_CHECK(output.is_equal("Type=unknown, value="));
  }

}

BOOST_AUTO_TEST_CASE(value_bool) {
  using dg::command::Value;

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

BOOST_AUTO_TEST_CASE(value_exceptions) {
  using dg::command::Value;

  Value value1;
  dg::command::EitherType anet(value1);
  output_test_stream output;
  
  // Check if the exception is working when calling intValue
  // while we are having a none.
  bool res = false;
  try {
    int aInt(anet);
    output << aInt;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);

  // Check if the exception is working when calling boolValue
  // while we are having a none.
  res = false;
  try {
    bool abool(anet);
    output << abool;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);
  
  // Check if the exception is working when calling unsignedintValue
  // while we are having a none.
  res = false;
  try {
    unsigned int aint(anet);
    output << aint;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);

  // Check if the exception is working when calling doubleValue
  // while we are having a none.
  res = false;
  try {
    double adouble(anet);
    output << adouble;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);

  // Check if the exception is working when calling floatValue
  // while we are having a none.
  res = false;
  try {
    float afloat(anet);
    output  << afloat;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);

  // Check if the exception is working when calling stringValue
  // while we are having a none.
  res = false;
  try {
    std::string astring(anet);
    output << astring;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);

  // Check if the exception is working when calling vectorValue
  // while we are having a none.
  res = false;
  try {
    dg::Vector avector;
    avector = anet;
    output << avector;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);

  // Check if the exception is working when calling matrixXdValue
  // while we are having a none.
  res = false;
  try {
    Eigen::MatrixXd amatrixXd;
    amatrixXd = anet;
    output << amatrixXd;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);
  
  // Check if the exception is working when calling matrix4dValue
  // while we are having a none.
  res = false;
  try {
    Eigen::Matrix4d amatrix4d;
    amatrix4d = anet;
  } catch (const dg::ExceptionAbstract &aea) {
    res = (aea.getCode() == dg::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);
  
}

BOOST_AUTO_TEST_CASE(value_unsigned_int) {
  using dg::command::Value;

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
  using dg::command::Value;

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
  using dg::command::Value;

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
  using dg::command::Value;

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
  using dg::command::Value;

  dg::Vector avector1;
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
  using dg::command::Value;

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
  using dg::command::Value;

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
  using dg::command::Value;

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
