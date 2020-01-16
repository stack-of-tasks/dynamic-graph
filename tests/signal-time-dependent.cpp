// Copyright 2010 Thomas Moulard.
//
#include <boost/foreach.hpp>
#include <iostream>

#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/signal.h>

#define BOOST_TEST_MODULE signal_time_dependent

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

typedef dynamicgraph::SignalTimeDependent<double, int> sigDouble_t;
typedef dynamicgraph::SignalTimeDependent<std::string, int> sigString_t;

template <class T> class DummyClass {
public:
  std::string proname;
  std::list<sigDouble_t *> inputsig;
  std::list<sigString_t *> inputsigV;

  explicit DummyClass(const std::string &n)
      : proname(n), res(), call(), timedata() {}

  T &fun(T &res, int t) {
    ++call;
    timedata = t;

    BOOST_FOREACH (sigDouble_t *ptr, inputsig)
      ptr->access(timedata);

    BOOST_FOREACH (sigString_t *ptr, inputsigV)
      ptr->access(timedata);

    res = (*this)();
    return res;
  }

  void add(sigDouble_t &sig) { inputsig.push_back(&sig); }
  void add(sigString_t &sig) { inputsigV.push_back(&sig); }

  T operator()();

  T res;
  int call;
  int timedata;
};

template <> double DummyClass<double>::operator()() {
  res = call * timedata;
  return res;
}
template <> std::string DummyClass<std::string>::operator()() {
  std::ostringstream oss;
  oss << call * timedata;
  return oss.str();
}

template <class T> T DummyClass<T>::operator()() { return this->res; }

BOOST_AUTO_TEST_CASE(signaltimedependent) {
  DummyClass<double> pro1("pro1"), pro3("pro3"), pro5("pro5");
  DummyClass<std::string> pro2("pro2"), pro4("pro4"), pro6("pro6");

  sigDouble_t sig5("Sig5");
  sigString_t sig6("Sig6");

  sigString_t sig4(sig5, "Sig4");
  sigString_t sig2(sig4 << sig4 << sig4 << sig6, "Sig2");
  sigDouble_t sig3(sig2 << sig5 << sig6, "Sig3");
  sigDouble_t sig1(boost::bind(&DummyClass<double>::fun, &pro1, _1, _2),
                   sig2 << sig3, "Sig1");
  sigDouble_t sig7("Sig7");

  sig2.setFunction(boost::bind(&DummyClass<std::string>::fun, &pro2, _1, _2));
  sig3.setFunction(boost::bind(&DummyClass<double>::fun, &pro3, _1, _2));
  sig4.setFunction(boost::bind(&DummyClass<std::string>::fun, &pro4, _1, _2));
  sig5.setFunction(boost::bind(&DummyClass<double>::fun, &pro5, _1, _2));
  sig6.setFunction(boost::bind(&DummyClass<std::string>::fun, &pro6, _1, _2));

  pro1.add(sig2);
  pro1.add(sig3);
  pro2.add(sig4);
  pro2.add(sig4);
  pro2.add(sig4);
  pro3.add(sig2);
  pro4.add(sig5);
  pro2.add(sig6);
  pro3.add(sig5);
  pro3.add(sig6);

  sig5.setDependencyType(dynamicgraph::TimeDependency<int>::ALWAYS_READY);
  sig6.setDependencyType(dynamicgraph::TimeDependency<int>::BOOL_DEPENDENT);

  sig5.setPeriodTime(3);
  assert(sig5.getPeriodTime() == 3);

  sig6.setReady();

  {
    output_test_stream output;
    sig1.displayDependencies(output);
    BOOST_CHECK(
        output.is_equal("-- Sig:Sig1 (Type Fun) (t=0 (/1) )\n"
                        "   |-- Sig:Sig3 (Type Fun) (t=0 (/1) )\n"
                        "   |   |-- Sig:Sig6 (Type Fun) (ready=TRUE)\n"
                        "   |   |-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |   `-- Sig:Sig2 (Type Fun) (t=0 (/1) )\n"
                        "   |       |-- Sig:Sig6 (Type Fun) (ready=TRUE)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=0 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=0 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       `-- Sig:Sig4 (Type Fun) (t=0 (/1) )\n"
                        "   |           `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   `-- Sig:Sig2 (Type Fun) (t=0 (/1) )\n"
                        "       |-- Sig:Sig6 (Type Fun) (ready=TRUE)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=0 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=0 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       `-- Sig:Sig4 (Type Fun) (t=0 (/1) )\n"
                        "           `-- Sig:Sig5 (Type Fun) (A)"));
  }

  BOOST_CHECK(sig1.needUpdate(2));
  sig1.access(2);

  {
    output_test_stream output;
    sig1.displayDependencies(output);
    BOOST_CHECK(
        output.is_equal("-- Sig:Sig1 (Type Fun) (t=2 (/1) )\n"
                        "   |-- Sig:Sig3 (Type Fun) (t=2 (/1) )\n"
                        "   |   |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "   |   |-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |   `-- Sig:Sig2 (Type Fun) (t=2 (/1) )\n"
                        "   |       |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=2 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=2 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       `-- Sig:Sig4 (Type Fun) (t=2 (/1) )\n"
                        "   |           `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   `-- Sig:Sig2 (Type Fun) (t=2 (/1) )\n"
                        "       |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=2 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=2 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       `-- Sig:Sig4 (Type Fun) (t=2 (/1) )\n"
                        "           `-- Sig:Sig5 (Type Fun) (A)"));
  }
  sig2.access(4);

  {
    output_test_stream output;
    sig1.displayDependencies(output);
    BOOST_CHECK(
        output.is_equal("-- Sig:Sig1 (Type Fun) (t=2 (/1) )\n"
                        "   |-- Sig:Sig3 (Type Fun) (t=2 (/1) )\n"
                        "   |   |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "   |   |-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |   `-- Sig:Sig2 (Type Fun) (t=4 (/1) )\n"
                        "   |       |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       `-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "   |           `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   `-- Sig:Sig2 (Type Fun) (t=4 (/1) )\n"
                        "       |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       `-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "           `-- Sig:Sig5 (Type Fun) (A)"));
  }
  sig1.access(4);

  {
    output_test_stream output;
    sig1.displayDependencies(output);
    BOOST_CHECK(
        output.is_equal("-- Sig:Sig1 (Type Fun) (t=4 (/1) )\n"
                        "   |-- Sig:Sig3 (Type Fun) (t=4 (/1) )\n"
                        "   |   |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "   |   |-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |   `-- Sig:Sig2 (Type Fun) (t=4 (/1) )\n"
                        "   |       |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "   |       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   |       `-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "   |           `-- Sig:Sig5 (Type Fun) (A)\n"
                        "   `-- Sig:Sig2 (Type Fun) (t=4 (/1) )\n"
                        "       |-- Sig:Sig6 (Type Fun) (ready=FALSE)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       |-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "       |   `-- Sig:Sig5 (Type Fun) (A)\n"
                        "       `-- Sig:Sig4 (Type Fun) (t=4 (/1) )\n"
                        "           `-- Sig:Sig5 (Type Fun) (A)"));
  }

  sig1.needUpdate(6);
  sig1.needUpdate(6);
  output_test_stream output;
  sig1.writeGraph(output);
  BOOST_CHECK(output.is_equal(""));

  sig1.removeDependency(sig3);
  BOOST_CHECK(true);
  const double &avalue = sig1(6);
  output << avalue;
  BOOST_CHECK(true);
  /// Verify check compatibility
  try {
    sig1.checkCompatibility();
  }
  //  catch(double e)
  catch (...) {
    std::cout << "Message: test \n";
  }
  BOOST_CHECK(true);
}
