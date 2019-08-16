/* Copyright 2019, LAAS-CNRS
 *
 * Olivier Stasse
 *
 */
#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <dynamic-graph/process-list.hh>

#define BOOST_TEST_MODULE debug - trace

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

BOOST_AUTO_TEST_CASE(testMt) {
  dynamicgraph::CPU::System aSystem;

  // create and open a character archive for output
  std::ofstream ofs("cpu_state.dat");
  boost::archive::text_oarchive oa(ofs);
  oa << aSystem;

  for (unsigned int i = 0; i < 10; i++) {
    usleep(100000);
    aSystem.readProcStat();
  }
}
