/*
 * Copyright 2018,
 * Joseph Mirabel
 *
 * LAAS-CNRS
 *
 * This file is part of dynamic-graph.
 * dynamic-graph is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * dynamic-graph is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <dynamic-graph/real-time-logger.h>

#define BOOST_TEST_MODULE real_time_logger

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <boost/thread.hpp>
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
  BOOST_CHECK(spinNb == 9);

  rtl.front() << "This msg should be short." << '\n';
  rtl.spinOnce();
}

bool requestShutdown = false;
void spin (RealTimeLogger* logger)
{
  while (!requestShutdown || !logger->empty())
  {
    // If the logger did not write anything, it means the buffer is empty.
    // Do a pause
    if (!logger->spinOnce())
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
  }
}

BOOST_AUTO_TEST_CASE (multithread)
{
  RealTimeLogger rtl (10);
  rtl.addOutputStream (LoggerStreamPtr_t (new LoggerIOStream(std::cout)));

  boost::thread loggerThread (spin, &rtl);

  for (int i = 0; i < 10; ++i) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(20));
    rtl.front() << "Call number " << i << '\n';
    BOOST_CHECK (!rtl.full());
  }

  rtl.front() << "This call should appear in the output" << '\n';

  requestShutdown = true;
  loggerThread.join();
}
