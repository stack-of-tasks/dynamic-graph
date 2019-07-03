//
// Created by corentin on 7/3/19.
//

#include <boost/foreach.hpp>

#include <dynamic-graph/signal.h>


#define BOOST_TEST_MODULE signal_array

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

typedef dynamicgraph::SignalArray_const<int> sigInt_t;

template <>
class DummyClass
{
public:
    DEF_SIZE = 20;

protected:
    std::list<int> inputsig;
    unsigned int inputsig_S,inputsig_R;
};