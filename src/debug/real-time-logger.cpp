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

namespace dynamicgraph
{
  RealTimeLogger::RealTimeLogger (const std::size_t& bufferSize)
    : buffer_(bufferSize, NULL)
    , oss_ (NULL)
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
    if (outputs_.empty() || full()) {
      oss_.rdbuf(NULL);
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
}
