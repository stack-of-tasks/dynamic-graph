// -*- c++-mode -*-
// Copyright 2010 François Bleibel Thomas Moulard, Olivier Stasse
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// You should have received a copy of the GNU Lesser General Public License
// along with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_SIGNAL_CASTER_HH
# define DYNAMIC_GRAPH_SIGNAL_CASTER_HH
# include <map>
# include <typeinfo>
# include <iostream>
# include <vector>

#include <Eigen/Dense>

# include <boost/any.hpp>
# include <boost/format.hpp>
# include <boost/function/function1.hpp>
# include <boost/function/function2.hpp>
# include <boost/lexical_cast.hpp>
# include <boost/tuple/tuple.hpp>

# include <dynamic-graph/dynamic-graph-api.h>
# include "dynamic-graph/exception-signal.h"

namespace dynamicgraph
{
  /// This singleton class allows serialization of a number of objects into
  /// (disp) and from (cast) std i/o streams.
  ///
  /// The transformation is done at run-time, i.e. SignalCaster
  /// doesn't know about the type of objects it casts to.
  ///
  /// It also allows registering of user-defined casts. A cast is
  /// identified by the compiler. The mapping from a type to a
  /// serialization function is dynamic, hence it is more complex than
  /// a typical template-based compile-time resolve. So disp, cast and
  /// trace are costly functions and should be used as such.
  class DYNAMIC_GRAPH_DLLAPI SignalCaster
  {
  public:
    virtual ~SignalCaster ();
    /// Destroy the unique instance.
    static void destroy();
    /// Typedef of displayer functions that take an encapsulated 'any'
    /// object and displays, cast, or trace it on an output stream
    /// (serialization).
    typedef boost::function2<void, const boost::any&, std::ostream&>
      displayer_type;
    typedef boost::function1<boost::any, std::istringstream&> caster_type;
    typedef boost::function2<void, const boost::any&, std::ostream&>
      tracer_type;

    /// Get a reference to the unique object of the class.
    static SignalCaster* getInstance(void);
    /// Displays an object using a registered displayer function.
    void disp (const boost::any& object, std::ostream& os);
    /// Traces an object using a registered trace function.
    void trace (const boost::any& object, std::ostream& os);
    /// Casts an object using a registered cast function.
    boost::any cast (const std::type_info&, std::istringstream& iss);
    /// Registers a cast.
    void registerCast (const std::type_info& type, displayer_type displayer,
		       caster_type caster, tracer_type tracer);
    /// Unregister a cast.
    void unregisterCast (const std::type_info& type);
    /// Checks if there is a displayer registered with type_name.
    bool existsCast (const std::type_info& type) const;
    /// Return the list of type names registered.
    std::vector<std::string> listTypenames () const;
  private:
    /// Container for the three cast functions.
    typedef boost::tuple<displayer_type, caster_type, tracer_type>
      cast_functions_type;

    /// \brief Retrieve cast structure from its name.
    cast_functions_type& getCast (const std::string& type_name);

    /// This map associates the typename of objects and the corresponding
    /// using boost::function with 'compatible' syntax
    std::map<std::string, cast_functions_type> functions_;

    std::map<std::string, const std::type_info*> type_info_;

  private:
    explicit SignalCaster ();
    /// Pointer to the unique instance of the class.
    static SignalCaster* instance_;
  };

  ///The SignalCast registerer class. Can be used to automatically
  /// register a cast when instanced somewhere in a cpp file. Pass the
  /// typeid () of the type you want to register a cast to as the first
  /// argument. The code is provided here so the class does not need
  /// to be exported.
  class DYNAMIC_GRAPH_DLLAPI SignalCastRegisterer
  {
  public:
    inline SignalCastRegisterer (const std::type_info& type,
				 SignalCaster::displayer_type displayer,
				 SignalCaster::caster_type caster,
				 SignalCaster::tracer_type tracer)
    {
      SignalCaster::getInstance()->registerCast(type, displayer,
						caster, tracer);
    }
  };


  /// Global signal cast template (helper) functions
  ///
  /// Using these avoid using the typeid () operator and keeps the
  /// implementation details hidden.
  template<typename T>
  void signal_disp (const T& value, std::ostream& os)
  {
    SignalCaster::getInstance()->disp(value, os);
  }

  template<typename T>
  T signal_cast(std::istringstream& iss)
  {
    return boost::any_cast<T>(SignalCaster::getInstance()->cast(typeid(T),
								iss));
  }

  template<typename T>
  void signal_trace (const T& value, std::ostream& os)
  {
    SignalCaster::getInstance()->trace(value, os);
  }
  
  // operator >> between istringstream and Matrix
  template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
  Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& 
    operator>> (const std::istringstream& iss, Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& a)
  {
    const std::string s = iss.str();
    int row, col;
    unsigned int i=1,j;
    bool isMatrix = true;
	
    // init row and col
    row = atoi(&s.at(i));
    for(j=i; s.at(j)!=',';j++){}
    i=j+1;
    col = atoi(&s.at(i));
	
    if(col==1)
      isMatrix = false;
	  
    a.resize(row,col);
	
    if(isMatrix)
    {
      for(j=i; s.at(j)!='(';j++){}
      i=j+1;
      for(int rowIt=0; rowIt<row; rowIt++)
      {
        if(rowIt==0)
        {
          i=j+1;
        }else{
          for(j=i; s.at(j)!='(';j++){}
          i=j+1;
        }
        for(int colIt=0; colIt<col; colIt++)
        {
          a(rowIt,colIt)=atof(&s.at(i));
          if(rowIt != row-1 && colIt != col-1)
          {
            for(j=i; s.at(j)!=',';j++){}
            i=j+1;
          }
        }
      }
    }else{	// vector
      for(j=i; s.at(j)!='(';j++){}
      i=j+1;
      for(int rowIt=0;rowIt<row;rowIt++)
      {
        a(rowIt,0)=atof(&s.at(i));
        if(rowIt!=row-1)
        {
          for(j=i; s.at(j)!=',';j++){}
          i=j+1;
        }
      }
    }
	
    return a;
  }
} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_SIGNAL_CASTER_HH
