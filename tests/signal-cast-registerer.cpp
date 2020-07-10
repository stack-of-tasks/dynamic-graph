// Copyright 2010 Thomas Moulard.
//

#include <string>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "signal-cast-register-test.h"
#include <dynamic-graph/debug.h>
#include <dynamic-graph/eigen-io.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/linear-algebra.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-cast-helper.h>
#include <dynamic-graph/signal-caster.h>
#include <dynamic-graph/signal.h>

#include "signal-cast-registerer-libA.hh"
#include "signal-cast-registerer-libB.hh"

#define BOOST_TEST_MODULE signal_cast_registerer

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

using boost::test_tools::output_test_stream;

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;

struct EigenCastRegisterer_V : public dynamicgraph::SignalCastRegisterer {
  typedef Vector bnuVector;

  EigenCastRegisterer_V()
      : SignalCastRegisterer(typeid(bnuVector), dispVector, castVector,
                             traceVector) {}

  static boost::any castVector(std::istringstream &iss) {
    bnuVector res;
    iss >> res;
    return res;
  }

  static void dispVector(const boost::any &object, std::ostream &os) {
    const bnuVector &v = boost::any_cast<bnuVector>(object);
    os << "[ ";
    for (int i = 0; i < v.size(); ++i)
      os << v(i) << " ";
    os << " ];" << std::endl;
  }

  static void traceVector(const boost::any &object, std::ostream &os) {
    const bnuVector &v = boost::any_cast<bnuVector>(object);
    for (int i = 0; i < v.size(); ++i)
      os << v(i) << " ";
    os << std::endl;
  }
};

template <typename Derived>
struct EigenCastRegisterer_M : public dynamicgraph::SignalCastRegisterer {
  typedef Matrix bnuMatrix;

  EigenCastRegisterer_M()
      : SignalCastRegisterer(typeid(bnuMatrix), dispMatrix, castMatrix,
                             traceMatrix) {}

  static boost::any castMatrix(std::istringstream &iss) {
    bnuMatrix res;
    iss >> res;
    return res;
  }

  static void dispMatrix(const boost::any &object, std::ostream &os) {
    const bnuMatrix &m = boost::any_cast<bnuMatrix>(object);
    os << m << std::endl;
  }

  static void traceMatrix(const boost::any &object, std::ostream &os) {
    const bnuMatrix &m = boost::any_cast<bnuMatrix>(object);
    os << m << std::endl;
  }
};

EigenCastRegisterer_V myVectorCast;
EigenCastRegisterer_M<int> myMatrixCast;

// Define a new cast with a type that supports streaming operators to
// and from it (this could be automated with macros).
dynamicgraph::DefaultCastRegisterer<bool> myBooleanCast;

// Check standard double cast registerer.
BOOST_AUTO_TEST_CASE(standard_double_registerer) {
  dynamicgraph::Signal<double, int> mySignal("out");

  typedef std::pair<std::string, std::string> test_t;
  std::vector<test_t> values;

  values.push_back(std::make_pair("42.0", "42\n"));
  values.push_back(std::make_pair("42.5", "42.5\n"));
  values.push_back(std::make_pair("-12.", "-12\n"));

  // Double special values.
  // FIXME: these tests are failing :(
  values.push_back(std::make_pair("inf", "inf\n"));
  values.push_back(std::make_pair("-inf", "-inf\n"));
  values.push_back(std::make_pair("nan", "nan\n"));

  BOOST_FOREACH (const test_t &test, values) {
    // Set
    std::istringstream value(test.first);
    mySignal.set(value);

    // Get
    {
      output_test_stream output;
      mySignal.get(output);
      BOOST_CHECK(output.is_equal(test.second));
    }

    // Trace
    {
      output_test_stream output;
      mySignal.trace(output);
      BOOST_CHECK(output.is_equal(test.second));
    }
  }

  // Check invalid values.
  // FIXME: this test is failing for now.
  std::istringstream value("This is not a valid double.");
  BOOST_CHECK_THROW(mySignal.set(value), std::exception);
}

// Check a custom cast registerer for Boost uBLAS vectors.
BOOST_AUTO_TEST_CASE(custom_vector_registerer) {
  dynamicgraph::Signal<dynamicgraph::Vector, int> myVectorSignal("vector");

  /// Create a second local vector registerer to generate an exception.
  bool res = false;
  try {
    EigenCastRegisterer_V myVectorCast2;
  } catch (const ExceptionSignal &aes) {
    res = (aes.getCode() == ExceptionSignal::GENERIC);
  }
  BOOST_CHECK(res); // Uncomment this line to resolve the unused-but-set-variable warning to have a stable release in ros buildfarm

  // Print the signal name.
  {
    output_test_stream output;
    output << myVectorSignal;
    BOOST_CHECK(output.is_equal("Sig:vector (Type Cst)"));
  }

  for (unsigned int i = 0; i < 5; ++i) {
    Vector v = Vector::Unit(5, i);
    std::ostringstream os;
    os << v;
    std::istringstream ss("[5](" + os.str() + ")");

    // Set signal value.
    myVectorSignal.set(ss);

    // Print out signal value.
    output_test_stream output;
    myVectorSignal.get(output);

    boost::format fmt("[ %d %d %d %d %d  ];\n");
    fmt % (i == 0) % (i == 1) % (i == 2) % (i == 3) % (i == 4);

    BOOST_CHECK(output.is_equal(fmt.str()));
  }

  // Catch Exception of ss (not good input)

  // ss[0] != "["
  try {
    std::istringstream ss("test");
    myVectorSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[0] != \"[\"";
  }

  // ss[1] != %i
  try {
    std::istringstream ss("[test");
    myVectorSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[1] != %i";
  }

  // ss[2] != "]"
  try {
    std::istringstream ss("[5[");
    myVectorSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[2] != \"]\"";
  }

  // ss[3] != "("
  try {
    std::istringstream ss("[5]test");
    myVectorSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[3] != \"(\"";
  }

  // ss[4] != ' ' || ','
  try {
    std::istringstream ss("[5](1, ");
    myVectorSignal.set(ss);
  } catch (ExceptionSignal &e) {
    BOOST_ERROR("Can't happened");
  }

  // ss[-1] != ")"
  try {
    std::istringstream ss("[5](1,2,3,4,5]");
    myVectorSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[-1] != \")\"";
  }

  try {
    output_test_stream output;
    myVectorSignal.trace(output);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[-1] != \")\"";
  }
}

BOOST_AUTO_TEST_CASE(custom_matrix_registerer) {
  dynamicgraph::Signal<dynamicgraph::Matrix, int> myMatrixSignal("matrix");

  // Print the signal name.
  {
    output_test_stream output;
    output << myMatrixSignal;
    BOOST_CHECK(output.is_equal("Sig:matrix (Type Cst)"));
  }

  // Catch Exception of ss (not good input)

  // ss[0] != "["
  try {
    std::istringstream ss("test");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[0] != \"[\"";
  }

  // ss[1] != %i
  try {
    std::istringstream ss("[test");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[1] != %i";
  }

  // ss[2] != ","
  try {
    std::istringstream ss("[5[");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[2] != \",\"";
  }

  // ss[3] != %i
  try {
    std::istringstream ss("[5,c");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[3] != %i";
  }

  // ss[4] != "]"
  try {
    std::istringstream ss("[5,3[");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[4] != \"]\"";
  }

  // ss[5] != "("
  try {
    std::istringstream ss("[5,3]test");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[5] != \"(\"";
  }

  // ss[6] != "("
  try {
    std::istringstream ss("[5,3](test");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[6] != \"(\"";
  }

  // ss[8] != " " || ","
  try {
    std::istringstream ss("[5,3]((1,");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    BOOST_ERROR("Can't happened");
  }

  // ss[6+n] != ")"
  try {
    std::istringstream ss("[5,3]((1,2,3]");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << ("ss[6+n] != \")\"");
  }

  // ss[-3] != ")"
  try {
    std::istringstream ss("[5,1]((1)(2)(3[");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[5] != \")\"";
  }

  // ss[-3] != ")"
  try {
    std::istringstream ss("[5,1]((1)(2)(3)[");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[5] != \")\"";
  }

  // ss[-1]!= ")"
  try {
    std::istringstream ss("[3,1]((1)(2),(3)[");
    myMatrixSignal.set(ss);
  } catch (ExceptionSignal &e) {
    std::cout << "Test passed : ss[5] != \")\" and ignore \",\"";
  }

  //[...]((...))
}
