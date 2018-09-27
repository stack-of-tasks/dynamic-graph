// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
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

#ifndef DYNAMIC_GRAPH_SIGNAL_BASE_H
# define DYNAMIC_GRAPH_SIGNAL_BASE_H
# include <string>
# include <sstream>
# include <typeinfo>
# include <boost/noncopyable.hpp>

# include <dynamic-graph/fwd.hh>
# include <dynamic-graph/exception-signal.h>


namespace dynamicgraph
{

  /** \brief The base class for signals: not to be used as such.

      Signal values can be accessed programmatically using the access
      () or accessCopy () methods; the former directly accesses the
      value of the signal, which can involve an extra computation,
      while the latter accesses a cached value, or 'copy'.
  */
  template<class Time>
  class SignalBase : public boost::noncopyable
  {
  public:
    explicit SignalBase(std::string name = "")
      : name (name),
	signalTime (0),
	ready (false)
    {}

    virtual ~SignalBase ()
    {}

    /// \name Time
    /// \{
    virtual const Time& getTime () const
    {
      return signalTime;
    }

    virtual void setTime (const Time& t)
    {
      signalTime = t;
    }

    const bool& getReady () const
    {
      return ready;
    }

    const std::string& getName () const
    {
      return name;
    }

    void getClassName(std::string & aClassName) const
    { aClassName = typeid(this).name(); }

    virtual void setPeriodTime (const Time&)
    {}

    virtual Time getPeriodTime () const
    {
      return 1;
    }

    /// \}

    /// \name Dependencies
    /// \{

    virtual void addDependency (const SignalBase<Time>&)
    {}

    virtual void removeDependency (const SignalBase<Time>&)
    {}

    virtual void clearDependencies ()
    {}

    virtual bool needUpdate (const Time&) const
    {
      return ready;
    }

    inline void setReady (const bool sready = true)
    {
      ready = sready;
    }

    virtual std::ostream&
    writeGraph (std::ostream& os ) const
    {
      return os;
    }

    virtual std::ostream&
    displayDependencies (std::ostream& os,
			 const int = -1,
			 std::string space = "",
			 std::string next1 = "",
			 std::string = "" ) const
    {
      os << space << next1 << "-- ";
      display(os);
      return os;
    }

    /// \}

    /// \name Plug
    /// \{

    /* Plug the arg-signal on the <this> object. Plug-in is always
     * a descending operation (the actual <this> object will call the arg-signal
     * and not the opposite).
     */
    virtual void plug (SignalBase<Time>* sigarg)
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::PLUG_IMPOSSIBLE,
	 "Plug-in operation not possible with this signal. ",
	 "(while trying to plug %s on %s).",
	 sigarg->getName ().c_str (),
	 this->getName ().c_str () );
    }

    virtual void unplug ()
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::PLUG_IMPOSSIBLE,
	 "Plug-in operation not possible with this signal. ",
	 "(while trying to unplug %s).",this->getName ().c_str ());
    }

    virtual bool isPlugged () const
    {
      return  false;
    }

    virtual SignalBase<Time>* getPluged () const
    {
      return NULL;
    }

    virtual void setConstantDefault ()
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::PLUG_IMPOSSIBLE,
	 "Plug-in operation not possible with this signal. ",
	 "(while trying to save %s).",
	 this->getName ().c_str ());
    }

    /// \}

    /// \name Set
    /// \{

    /* Generic set function. Should be reimplemented by the specific
     * Signal.  Sets a signal value
     */
    virtual void set (std::istringstream&)
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::SET_IMPOSSIBLE,
	 "Set operation not possible with this signal. ",
	 "(while trying to set %s).",
	 this->getName ().c_str () );
    }

    virtual void get (std::ostream&) const
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::SET_IMPOSSIBLE,
	 "Get operation not possible with this signal. ",
	 "(while trying to get %s).",
	 this->getName ().c_str ());
    }

    virtual inline void recompute (const Time &)
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::SET_IMPOSSIBLE,
	 "Recompute operation not possible with this signal. ",
	 "(while trying to recompute %s).",
	 this->getName ().c_str ());
    }

    virtual void trace (std::ostream&) const
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::SET_IMPOSSIBLE,
	 "Trace operation not possible with this signal. ",
	 "(while trying to trace %s).",
	 this->getName ().c_str () );
    }

    /// \}

    /// \name Display
    /// \{

    virtual std::ostream& display (std::ostream& os) const
    {
      os << "Sig:" << name;
      return os;
    }

    std::string shortName () const
    {
      std::istringstream iss (name);
      const int SIZE = 128;
      char buffer[SIZE];
      while (iss.good ())
	{
	  iss.getline(buffer,SIZE,':');
	}
      const std::string res (buffer);
      return res;
    }
    /// \}

    /// \name Information providers
    /// \{

    virtual void ExtractNodeAndLocalNames (std::string& LocalName,
					   std::string & NodeName) const
    {
      std::string fullname = this->getName ();

      size_t IdxPosLocalName = fullname.rfind (":");
      LocalName = fullname.substr (IdxPosLocalName + 1,
				   fullname.length () - IdxPosLocalName + 1);
      size_t IdxPosNodeNameStart = fullname.find ("(");
      size_t IdxPosNodeNameEnd = fullname.find (")");
      NodeName = fullname.substr (IdxPosNodeNameStart + 1,
				  IdxPosNodeNameEnd - IdxPosNodeNameStart - 1);
    }

    /// \}

    /// \name Test
    /// \{
    virtual void checkCompatibility ()
    {
      DG_THROW ExceptionSignal
	(ExceptionSignal::PLUG_IMPOSSIBLE,
	 "Abstract signal not compatible with anything.",
	 "(while trying to plug <%s>).",
	 this->getName ().c_str ());
    }
    /// \}

  protected:
    std::string name;
    Time signalTime;
    bool ready;
  };

  /// Forward to a virtual fonction.
  template<class Time>
  std::ostream& operator<< (std::ostream& os, const SignalBase<Time>& sig)
  {
    return sig.display(os);
  }
} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_SIGNAL_BASE_H
