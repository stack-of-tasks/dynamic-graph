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

#if BOOST_VERSION >= 105900
#include <boost/test/tools/output_test_stream.hpp>
#else
#include <boost/test/output_test_stream.hpp>
#endif
#include <boost/test/unit_test.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

using namespace dynamicgraph;

BOOST_AUTO_TEST_CASE(monothread) {
  RealTimeLogger rtl(10);
  rtl.addOutputStream(LoggerStreamPtr_t(new LoggerIOStream(std::cout)));
  for (int i = 0; i < 9; ++i)
    rtl.front() << "Call number " << i << '\n';
  BOOST_CHECK(rtl.full());
  rtl.front() << "This call should not appear in the output" << '\n';

  rtl.spinOnce();
  BOOST_CHECK(!rtl.full());
  rtl.front() << "This call should appear in the output" << '\n';

  int spinNb = 0;
  while (rtl.spinOnce()) {
    spinNb++;
  }
  BOOST_CHECK_EQUAL(spinNb, 9);

  rtl.front() << "This msg should be short." << '\n';
  rtl.spinOnce();
}

BOOST_AUTO_TEST_CASE(multithread) {
  // The part of the code changing priority will only be effective
  // if this test is run as root. Otherwise it behaves like a classical thread.
  // Test confirms that in this case, it runs with a priority -51
  // and that the thread for logging is running on SCHED_OTHER
  // with a nice priority (20).
  int threadPolicy;
  struct sched_param threadParam;
  if (pthread_getschedparam(pthread_self(), &threadPolicy, &threadParam) == 0) {
    threadPolicy = SCHED_RR;
    threadParam.sched_priority = 50;
    pthread_setschedparam(pthread_self(), threadPolicy, &threadParam);
  }

  RealTimeLogger &rtl = RealTimeLogger::instance();
  dgADD_OSTREAM_TO_RTLOG(std::cout);

  for (std::size_t i = 0; i < rtl.getBufferSize() - 1; ++i)
    dgRTLOG() << "Call number " << i << '\n';
  for (std::size_t i = 0; i < 12; ++i) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
    dgRTLOG() << "Call number " << i << std::endl;
    BOOST_CHECK(!rtl.full());
  }

  dgRTLOG() << "This call should appear in the output" << '\n';

  RealTimeLogger::destroy();
}
