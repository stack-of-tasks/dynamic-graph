// Copyright 2010, Thomas Moulard, JRL, CNRS/AIST
//

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
