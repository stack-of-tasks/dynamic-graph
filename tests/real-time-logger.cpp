/*
 * Copyright 2018,
 * Joseph Mirabel
 *
 * LAAS-CNRS
 *
 */

#include <iostream>

#define ENABLE_RT_LOG
#include <dynamic-graph/real-time-logger.h>

#define BOOST_TEST_MODULE real_time_logger

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace dynamicgraph;

BOOST_AUTO_TEST_CASE (monothread)
{
  RealTimeLogger rtl (10);
  rtl.addOutputStream (LoggerStreamPtr_t (new LoggerIOStream(std::cout)));
  for (int i = 0; i < 9; ++i) rtl.front() << "Call number " << i << '\n';
  BOOST_CHECK (rtl.full());
  rtl.front() << "This call should not appear in the output" << '\n';

  rtl.spinOnce();
  BOOST_CHECK (!rtl.full());
  rtl.front() << "This call should appear in the output" << '\n';

  int spinNb = 0;
  while (rtl.spinOnce()) { spinNb++; }
  BOOST_CHECK_EQUAL(spinNb, 9);

  rtl.front() << "This msg should be short." << '\n';
  rtl.spinOnce();
}

BOOST_AUTO_TEST_CASE (multithread)
{
  RealTimeLogger& rtl = RealTimeLogger::instance();
  dgADD_OSTREAM_TO_RTLOG (std::cout);

  for (std::size_t i = 0; i < rtl.getBufferSize()-1; ++i)
    dgRTLOG() << "Call number " << i << '\n';
  for (std::size_t i = 0; i < 12; ++i) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
    dgRTLOG() << "Call number " << i << std::endl;
    BOOST_CHECK (!rtl.full());
  }

  dgRTLOG() << "This call should appear in the output" << '\n';

  RealTimeLogger::destroy();
}
