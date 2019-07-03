//
// Created by Corentin on 7/3/19.
//

#include <sstream>
#include <dynamic-graph/command-direct-setter.h>


#define BOOST_TEST_MODULE command

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

struct Command : public dynamicgraph::command::DirectSetter
{

};