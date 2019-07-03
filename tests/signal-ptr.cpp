// Copyright 2010 Thomas Moulard.
//

#include <string>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal.h>
#include <dynamic-graph/pool.h>
#include <iostream>

#define BOOST_TEST_MODULE signal_ptr


#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

typedef dynamicgraph::SignalPtr<double, int> sigDouble_t;

template<class T>
class DummyClass
{
public:

    std::string proname;











int main( void )
{
  using namespace dynamicgraph;
  using std::cout;

  Signal<double,int> sig("sig");
  SignalPtr<double,int> sigPtrA(NULL,"sigPtrA"),sigPtrB(NULL,"sigPtrB");

  sig.setConstant( 1.56 );

  SignalBase<int> & sigRef = sig;
  SignalBase<int> & sigPtrARef = sigPtrA, & sigPtrBRef = sigPtrB;
  sigPtrARef.plug( & sigRef );
  sigPtrBRef.plug( & sigPtrARef );

  cout << "Sig = ";  sigRef.get(cout); cout << std::endl;
  cout << "SigPtrA = ";  sigPtrARef.get(cout); cout << std::endl;
  cout << "SigPtrB = ";  sigPtrBRef.get(cout); cout << std::endl;

  return 0;
}


