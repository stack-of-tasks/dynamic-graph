// Copyright 2010 Thomas Moulard.
//

#include <string>

#include <boost/foreach.hpp>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/signal.h>
#include <iostream>

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <string>

using boost::test_tools::output_test_stream;

typedef dynamicgraph::SignalTimeDependent<double, int> sigDouble_t;
typedef dynamicgraph::SignalTimeDependent<std::string, int> sigString_t;

using namespace dynamicgraph;
using std::cout;

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

BOOST_AUTO_TEST_CASE(normal_cst_test) {
  SignalPtr<double, int> sigNotPlug(NULL, "sigNotPlug");
  const SignalPtr<double, int> cstSigNotPlug(NULL, "sigNotPlug");

  try {
    sigNotPlug.getPtr();
  } catch (ExceptionSignal &e) {
    cout << "Error catch" << std::endl;
  }

  // Test getPtr without plug
  /// This create a ExceptionSignal::NOT_INITIALIZED
  bool res = false;
  try {
    //    Signal<double, int> * r =
    sigNotPlug.getPtr();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::NOT_INITIALIZED);
  }
  BOOST_CHECK(res);

  /// Testing const getPtr() interface: no plug case
  try {
    cstSigNotPlug.getPtr();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::NOT_INITIALIZED);
  }
  BOOST_CHECK(res);

  /// Test needUpdate without plug
  res = (sigNotPlug.needUpdate(5) == false);
  BOOST_CHECK(res);
  sigNotPlug.getTime();
  output_test_stream output;
  sigNotPlug.display(output);
  cstSigNotPlug.display(output);

  /// Testing getAbsatractPtr() interface: no plug
  res = false;
  try {
    sigNotPlug.getAbstractPtr();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::NOT_INITIALIZED);
  }
  BOOST_CHECK(res);

  /// Testing const getAbstractPtr() interface: no plug case
  try {
    cstSigNotPlug.getAbstractPtr();
  } catch (const ExceptionSignal &aea) {
    res = (aea.getCode() == ExceptionSignal::NOT_INITIALIZED);
  }
  BOOST_CHECK(res);

  try {
    sigNotPlug.checkCompatibility();
  } catch (...) {
  }
  BOOST_CHECK(res);
}

BOOST_AUTO_TEST_CASE(normal_test) {
  Signal<double, int> sig("sig");
  Signal<int, int> sigint("sig");
  Signal<std::string, int> sigstr("sig_str");
  SignalPtr<double, int> sigPtrA(NULL, "sigPtrA"), sigPtrB(NULL, "sigPtrB");
  SignalPtr<double, int> sigPtrAbstract(NULL, "sigPtrAbstract");
  DummyClass<double> pro3("pro3");

  sig.setConstant(1.56);
  sig.recompute(2);
  std::string name = "sig";
  sig.getClassName(name);
  std::string test = "test";
  try {
    sig.getClassName(test);
  } catch (ExceptionSignal &e) {
    e.getExceptionName();
  }
  BOOST_CHECK(true);

  sigPtrA.setFunction(boost::bind(&DummyClass<double>::fun, &pro3, _1, _2));
  sigPtrA.recompute(3);

  /// Plugging signal.
  SignalBase<int> &sigRef = sig, sigBase("sigBase");
  SignalBase<int> &sigPtrARef = sigPtrA, &sigPtrBRef = sigPtrB,
                  &sigPtrAbstractRef = sigPtrAbstract;
  sigPtrARef.plug(0);
  sigPtrARef.plug(&sigRef);
  sigPtrBRef.plug(&sigPtrARef);
  /// Try to plug an incompatible signal.
  /// leave
  bool res = false;
  try {
    sigPtrARef.plug(&sigstr);
  } catch (const ExceptionSignal &aes) {
    res = (aes.getCode() == ExceptionSignal::PLUG_IMPOSSIBLE);
  }
  BOOST_CHECK(res);

  /// Plug the signal.
  sigPtrAbstractRef.plug(&sigRef);
  sigPtrA.getPtr();
  BOOST_CHECK(true);
  try {
    sigPtrARef.checkCompatibility();
  } catch (const ExceptionSignal &aes) {
    /// Should be NOT_INITIALIZED becase the last plug
    /// on sigstr failed.
    res = (aes.getCode() == ExceptionSignal::NOT_INITIALIZED);
  } catch (const std::exception &e) {
    std::cout << "Standard Exception:" << e.what() << std::endl;
  } catch (...) {
    std::cout << "Anything else: " << std::endl;
  }
  sigPtrA.needUpdate(5);
  BOOST_CHECK(true);

  int ltime = sigPtrA.getTime();
  sigPtrA.getPluged();
  sigPtrA(ltime);
  BOOST_CHECK(true);

  sigPtrB.getPtr();
  /// Test sigPtrAbstract with a normal plug.
  res = false;
  try {
    sigPtrAbstract.getAbstractPtr();
  } catch (ExceptionSignal &aes) {
    /// Should be NOT_INITIALIZED becase the last plug
    /// on sigstr failed.
    std::cout << "Code: " << aes.getCode() << std::endl;
    res = (aes.getCode() == ExceptionSignal::NOT_INITIALIZED);
  } catch (...) {
    std::cout << "Anything else with sigPtrAbstract.getAbstractPtr()"
              << std::endl;
  }
  BOOST_CHECK(true);

  /// Test the case where the plug ref is zero.
  sigPtrAbstractRef.plug(0);
  BOOST_CHECK(true);

  assert(sigRef.isPlugged() != true);
  SignalBase<int> *t = sigRef.getPluged();
  // assert(sigPtrA.get()=false);

  // TODO Can't check if the constant change
  sigPtrA.setConstantDefault(1.2);
  // getconstant
  sigPtrA.setConstantDefault();
  // getconstant
  sigPtrA.setConstant(3.4);
  // getconstant
  double tab_D[2];
  tab_D[0] = 1.2;
  tab_D[1] = 3.4;
  sigPtrA.setReference(tab_D, NULL);
  sigPtrA.access(5);
  output_test_stream output;
  sigPtrA.display(output);
  sigPtrA.setReferenceNonConstant(tab_D, NULL);
  sigPtrA.access(5);
  sigPtrA.display(output);

  // getreference
  sigPtrA.operator=(1.2);
  // getconstant
  sigPtrA.displayDependencies(output);

  cout << t << std::endl;
  cout << "Sig = ";
  sigRef.get(cout);
  cout << std::endl;
  cout << "SigPtrA = ";
  sigPtrARef.get(cout);
  cout << std::endl;
  cout << "SigPtrB = ";
  sigPtrBRef.get(cout);
  cout << std::endl;

  sigPtrA.unplug();
}

BOOST_AUTO_TEST_CASE(plug_signal_string) {
  Signal<std::string, int> outSig("output");
  SignalPtr<std::string, int> inSig(NULL, "input");

  Signal<dynamicgraph::Vector, int> outSigVec("outputVec");
  SignalPtr<dynamicgraph::Vector, int> inSigVec(NULL, "inputVec");

  std::string str("two words");
  outSig.setConstant(str);
  inSig.plug(&outSig);
  inSig.recompute(1);
  std::ostringstream os1;
  inSig.get(os1);
  std::string res(os1.str());
  BOOST_CHECK(res == str);

  dynamicgraph::Vector aVec;
  aVec.resize(5);
  aVec(0) = 1.0;
  aVec(1) = 2.0;
  aVec(2) = 3.0;
  aVec(3) = 4.0;
  aVec(4) = 5.0;
  outSigVec.setConstant(aVec);
  inSigVec.plug(&outSigVec);
  inSigVec.recompute(1);
  output_test_stream output;
  inSigVec.get(output);
  BOOST_CHECK(output.is_equal("1\n2\n3\n4\n5\n"));

  Signal<std::string, int> s("signal");
  std::ostringstream os2;
  s.setConstant(str);
  os2.clear();
  s.get(os2);
  res = os2.str();
  std::cout << "res=" << res << std::endl;
  BOOST_CHECK(res == str);
}

BOOST_AUTO_TEST_CASE(set_signal_string) {
  Signal<std::string, int> s("signal");
  std::string str("");
  std::ostringstream os;
  os << str;
  std::istringstream value(os.str());
  try {
    s.set(value);
  }
  catch(const std::exception& exc)
  {
    std::cout << exc.what() << std::endl;
    BOOST_CHECK(!"Tentative to set signal to empty string");
  }
}
