// Copyright 2010 Thomas Moulard.
//

#include <string>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal.h>
#include <iostream>

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <string>

using namespace dynamicgraph;
using std::cout;

BOOST_AUTO_TEST_CASE(normal_test) {
  Signal<double, int> sig("sig");
  SignalPtr<double, int> sigPtrA(NULL, "sigPtrA"), sigPtrB(NULL, "sigPtrB");
  SignalPtr<double, int> sigNotPlug(NULL, "sigNotPlug");
  SignalPtr<double, int> sigPtrAbstract(NULL, "sigPtrAbstract");

  try {
    sigNotPlug.getPtr();
  } catch (ExceptionSignal e) {
    cout << "Error catch" << std::endl;
  }

  sig.setConstant(1.56);
  sig.recompute(2);
  std::string name = "sig";
  sig.getClassName(name);
  std::string test = "test";
  try {
    sig.getClassName(test);
  } catch (ExceptionSignal e) {
    e.getExceptionName();
  }

  SignalBase<int> &sigRef = sig;
  SignalBase<int> &sigPtrARef = sigPtrA, &sigPtrBRef = sigPtrB,
                  &sigPtrAbstractRef = sigPtrAbstract;
  sigPtrARef.plug(&sigRef);
  sigPtrBRef.plug(&sigPtrARef);

  // TODO here
  sigPtrAbstractRef.plug(&sigRef);
  sigPtrA.getPtr();
  sigPtrA.getPluged();
  sigPtrB.getPtr();
  sigPtrAbstract.getAbstractPtr();

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
  // getreference
  sigPtrA.operator=(1.2);
  // getconstant

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
}

BOOST_AUTO_TEST_CASE (plug_signal_string)
{
  Signal<std::string, int> outSig("output");
  SignalPtr<std::string, int> inSig (NULL, "input");

  std::string str ("two words");
  outSig.setConstant(str);
  inSig.plug (&outSig);
  inSig.recompute(1);
  std::ostringstream os1;
  inSig.get (os1);
  std::string res (os1.str ());
  BOOST_CHECK (res == str);

  Signal<std::string, int> s ("signal");
  std::ostringstream os2;
  s.setConstant (str);
  os2.clear ();
  s.get (os2);
  res = os2.str ();
  std::cout << "res=" << res << std::endl;
  BOOST_CHECK (res == str);
}
