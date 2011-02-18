// Copyright 2010, Thomas Moulard, JRL, CNRS/AIST
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

#ifndef DYNAMIC_GRAPH_NULL_PTR_HH
# define DYNAMIC_GRAPH_NULL_PTR_HH

namespace dynamicgraph
{
  const class
  {
  public:
    template<class T>
    operator T*() const
    {
      return 0;
    }
    
    template<class C, class T>
    operator T C::*() const
    {
      return 0;
    }
  private:
    void operator&() const;
  } nullptr = {};

} // end of namespace dynamicgraph.

#endif //! DYNAMIC_GRAPH_NULL_PTR_HH
