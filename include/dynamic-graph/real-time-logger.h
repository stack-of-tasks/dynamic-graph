// -*- mode: c++ -*-
// Copyright 2018, Joseph Mirabel LAAS-CNRS
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// dynamic-graph. If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_LOGGER_REAL_TIME_H
# define DYNAMIC_GRAPH_LOGGER_REAL_TIME_H
# include <sstream>
# include <vector>

# include <boost/circular_buffer.hpp>
# include <boost/shared_ptr.hpp>
# include <boost/thread/mutex.hpp>

# include <dynamic-graph/config.hh>
# include <dynamic-graph/debug.h>


namespace dynamicgraph
{
  /// \ingroup debug
  ///
  /// \brief Stream for the real-time logger.
  ///
  /// You should inherit from this class in order to redirect the logs where you
  /// want.
  /// \sa LoggerIOStream
  class LoggerStream
  {
    public:
      virtual void write (const char* c) = 0;
  };

  /// Write to an ostream object.
  ///
  /// The easieast is to use the macro \ref dgADD_OSTREAM_TO_RTLOG(ostr) where
  /// `ostr` can be `std::cout` or an std::ofstream...
  class LoggerIOStream : public LoggerStream
  {
    public:
      LoggerIOStream (std::ostream& os) : os_ (os) {}
      virtual void write (const char* c) { os_ << c; }
    private:
      std::ostream& os_;
  };
  typedef boost::shared_ptr <LoggerStream> LoggerStreamPtr_t;

  class RealTimeLogger;

  /// \cond DEVEL
  /// \brief write entries to intenal buffer.
  ///
  /// The entry starts when an instance is created and ends when is is deleted.
  /// This class is only used by RealTimeLogger.
  class RTLoggerStream
  {
    public:
      RTLoggerStream (RealTimeLogger* logger, std::ostream& os) : logger_(logger), os_ (os) {}
      template <typename T> inline RTLoggerStream& operator<< (T  t) { if (logger_!=NULL) os_ << t; return *this; }
      inline RTLoggerStream& operator<< (std::ostream& (*pf)(std::ostream&)) { if (logger_!=NULL) os_ << pf; return *this; }

      ~RTLoggerStream();
    private:

      RealTimeLogger* logger_;
      std::ostream& os_;
  };
  /// \endcond DEVEL

  /// \ingroup debug
  ///
  /// \brief Main class of the real-time logger.
  ///
  /// It is intended to be used like this:
  /// \code
  /// #define ENABLE_RT_LOG
  /// #include <dynamic-graph/real-time-logger.h>
  ///
  /// // Somewhere in the main function of your executable
  /// int main (int argc, char** argv) {
  ///   dgADD_OSTREAM_TO_RTLOG (std::cout);
  /// }
  ///
  /// // Somewhere in your library
  /// dgRTLOG() << "your message. Prefer to use \n than std::endl."
  /// \endcode
  ///
  /// \note Thread safety. This class expects to have:
  /// - only one reader: the one who take the log entries and write them somewhere.
  /// - one writer at a time. Writing to the logs is **never** a blocking
  ///   operation. If the resource is busy, the log entry is discarded.
  class DYNAMIC_GRAPH_DLLAPI RealTimeLogger
  {
  public:
    static RealTimeLogger& instance();

    static void destroy();

    /// \todo add an argument to preallocate the internal string to a given size.
    RealTimeLogger (const std::size_t& bufferSize);

    inline void clearOutputStreams () { outputs_.clear(); }

    inline void addOutputStream (const LoggerStreamPtr_t& os) { outputs_.push_back(os); }

    /// Write next message to output.
    /// It does nothing if the buffer is empty.
    /// \return true if it wrote something
    bool spinOnce ();

    /// Return an object onto which a real-time thread can write.
    /// The message is considered finished when the object is destroyed.
    RTLoggerStream front();

    inline void frontReady() { backIdx_ = (backIdx_+1) % buffer_.size(); wmutex.unlock(); }

    inline bool empty () const
    {
      return frontIdx_ == backIdx_;
    }

    inline bool full () const
    {
      return ((backIdx_ + 1) % buffer_.size()) == frontIdx_;
    }

    inline std::size_t size () const
    {
      if (frontIdx_ <= backIdx_)
        return backIdx_ - frontIdx_;
      else
        return backIdx_ + buffer_.size() - frontIdx_;
    }

    inline std::size_t getBufferSize () { return buffer_.size(); }

    ~RealTimeLogger ();

  private:

    struct Data {
      std::stringbuf buf;
    };

    std::vector<LoggerStreamPtr_t> outputs_;
    std::vector<Data*> buffer_;
    /// Index of the next value to be read.
    std::size_t frontIdx_;
    /// Index of the slot where to write next value (does not contain valid data).
    std::size_t backIdx_;
    std::ostream oss_;

    /// The writer mutex.
    boost::mutex wmutex;
    std::size_t nbDiscarded_;

    struct thread;

    static RealTimeLogger* instance_;
    static thread* thread_;
  };
} // end of namespace dynamicgraph

#ifdef ENABLE_RT_LOG
# define dgADD_OSTREAM_TO_RTLOG(ostr) ::dynamicgraph::RealTimeLogger::instance() \
  .addOutputStream(::dynamicgraph::LoggerStreamPtr_t(new ::dynamicgraph::LoggerIOStream(ostr)))
# define dgRTLOG() ::dynamicgraph::RealTimeLogger::instance().front()
#else // ENABLE_RT_LOG
# define dgADD_OSTREAM_TO_RTLOG(ostr) struct __end_with_semicolon
# define dgRTLOG() if (1) ; else __null_stream()
#endif

#endif //! DYNAMIC_GRAPH_LOGGER_REAL_TIME_H
