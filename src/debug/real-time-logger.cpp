/*
 * Copyright 2018,
 * Joseph Mirabel
 *
 * LAAS-CNRS
 *
 */

#include <dynamic-graph/real-time-logger.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

namespace dynamicgraph {
RealTimeLogger::RealTimeLogger(const std::size_t &bufferSize)
    : buffer_(bufferSize, NULL), frontIdx_(0), backIdx_(0), oss_(NULL),
      nbDiscarded_(0) {
  for (std::size_t i = 0; i < buffer_.size(); ++i)
    buffer_[i] = new Data;
}

RealTimeLogger::~RealTimeLogger() {
  // Check that we are not spinning...
  for (std::size_t i = 0; i < buffer_.size(); ++i)
    delete buffer_[i];
}

bool RealTimeLogger::spinOnce() {
  if (empty())
    return false;
  Data *data = buffer_[frontIdx_];
  frontIdx_ = (frontIdx_ + 1) % buffer_.size();
  std::string str = data->buf.str();
  // It is important to pass str.c_str() and not str
  // because the str object may contains a '\0' so
  // str.size() may be different from strlen(str.c_str())
  for (std::size_t i = 0; i < outputs_.size(); ++i)
    outputs_[i]->write(str.c_str());
  return true;
}

RTLoggerStream RealTimeLogger::front() {
  // If no output or if buffer is full, discard message.
  if (outputs_.empty() || full()) {
    nbDiscarded_++;
    return RTLoggerStream(NULL, oss_);
  }
  bool alone = wmutex.try_lock();
  // If someone is writting, discard message.
  if (!alone) {
    nbDiscarded_++;
    return RTLoggerStream(NULL, oss_);
  }
  Data *data = buffer_[backIdx_];
  // backIdx_ = (backIdx_+1) % buffer_.size();
  // Reset position of cursor
  data->buf.pubseekpos(0);
  oss_.rdbuf(&data->buf);
  return RTLoggerStream(this, oss_);
}

struct RealTimeLogger::thread {
  bool requestShutdown_;
  int threadPolicy_;
  int threadPriority_;
  bool changedThreadParams;
  boost::thread t_;

  explicit thread(RealTimeLogger *logger)
      : requestShutdown_(false), threadPolicy_(SCHED_OTHER), threadPriority_(0),
        changedThreadParams(true), t_(&thread::spin, this, logger) {}

  //  void setThreadPolicy(int policy) {
  //  threadPolicy_ = policy;
  //  changedThreadParams = true;
  // }

  // void setPriority(int priority) {
  //    threadPriority_ = priority;
  //    changedThreadParams = true;
  //  }

  //  int getThreadPolicy() { return threadPolicy_; }
  // int getThreadPriority() { return threadPriority_; }

  void changeThreadParams() {
    int threadPolicy;
    struct sched_param threadParam;
    if (pthread_getschedparam(pthread_self(), &threadPolicy, &threadParam) ==
        0) {
      threadPolicy = threadPolicy_;
      threadParam.sched_priority = threadPriority_;
      if (threadParam.sched_priority < sched_get_priority_min(threadPolicy))
        threadParam.sched_priority = sched_get_priority_min(threadPolicy);

      pthread_setschedparam(pthread_self(), threadPolicy, &threadParam);
      changedThreadParams = false;
    }
  }

  void spin(RealTimeLogger *logger) {
    // Change the thread's scheduler from real-time to normal
    // and reduce its priority

    while (!requestShutdown_ || !logger->empty()) {
      // If the logger did not write anything, it means the buffer is empty.
      // Do a pause
      if (!logger->spinOnce())
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
      if (changedThreadParams)
        changeThreadParams();
    }
  }
};

RealTimeLogger *RealTimeLogger::instance_ = NULL;
RealTimeLogger::thread *RealTimeLogger::thread_ = NULL;

RealTimeLogger &RealTimeLogger::instance() {
  if (instance_ == NULL) {
    instance_ = new RealTimeLogger(1000);
    thread_ = new thread(instance_);
  }
  return *instance_;
}

void RealTimeLogger::destroy() {
  if (instance_ == NULL)
    return;
  thread_->requestShutdown_ = true;
  thread_->t_.join();
  delete instance_;
  delete thread_;
}
} // namespace dynamicgraph
