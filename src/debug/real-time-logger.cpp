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

#include <dynamic-graph/real-time-logger.h>

#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace dynamicgraph
{
  RealTimeLogger::RealTimeLogger (const std::size_t& bufferSize)
    : buffer_(bufferSize, NULL)
    , frontIdx_ (0)
    , backIdx_ (0)
    , oss_ (NULL)
    , nbDiscarded_ (0)
  {
    for (std::size_t i = 0; i < buffer_.size(); ++i)
      buffer_[i] = new Data;
  }

  RealTimeLogger::~RealTimeLogger ()
  {
    // Check that we are not spinning...
    for (std::size_t i = 0; i < buffer_.size(); ++i) delete buffer_[i];
  }

  bool RealTimeLogger::spinOnce ()
  {
    if (empty()) return false;
    Data* data = buffer_[frontIdx_];
    frontIdx_ = (frontIdx_ + 1) % buffer_.size();
    std::string str = data->buf.str();
    // It is important to pass str.c_str() and not str
    // because the str object may contains a '\0' so
    // str.size() may be different from strlen(str.c_str())
    for (std::size_t i = 0; i < outputs_.size(); ++i)
      outputs_[i]->write (str.c_str());
    return true;
  }

  RTLoggerStream RealTimeLogger::front ()
  {
    // If no output or if buffer is full, discard message.
    if (outputs_.empty() || full()) {
      nbDiscarded_++;
      return RTLoggerStream (NULL, oss_);
    }
    bool alone = wmutex.try_lock();
    // If someone is writting, discard message.
    if (!alone) {
      nbDiscarded_++;
      return RTLoggerStream (NULL, oss_);
    }
    Data* data = buffer_[backIdx_];
    //backIdx_ = (backIdx_+1) % buffer_.size();
    // Reset position of cursor
    data->buf.pubseekpos(0);
    oss_.rdbuf(&data->buf);
    return RTLoggerStream (this, oss_);
  }

  RTLoggerStream::~RTLoggerStream()
  {
    os_ << std::ends;
    if (logger_ != NULL) logger_->frontReady();
  }

  struct RealTimeLogger::thread
  {
    bool requestShutdown_;
    boost::thread t_;

    thread (RealTimeLogger* logger)
      : requestShutdown_ (false)
      , t_ (&thread::spin, this, logger)
    {}

    void spin (RealTimeLogger* logger)
    {
      while (!requestShutdown_ || !logger->empty())
      {
        // If the logger did not write anything, it means the buffer is empty.
        // Do a pause
        if (!logger->spinOnce())
          boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
    }
  };

  RealTimeLogger* RealTimeLogger::instance_ = NULL;
  RealTimeLogger::thread* RealTimeLogger::thread_ = NULL;

  RealTimeLogger& RealTimeLogger::instance()
  {
    if (instance_ == NULL) {
      instance_ = new RealTimeLogger (1000);
      thread_ = new thread (instance_);
    }
    return *instance_;
  }

  void RealTimeLogger::destroy ()
  {
    if (instance_ == NULL) return;
    thread_->requestShutdown_ = true;
    thread_->t_.join();
    delete instance_;
    delete thread_;
  }
}
