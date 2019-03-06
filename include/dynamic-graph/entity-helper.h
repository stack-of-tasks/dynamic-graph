/*
 * Copyright 2011, Nicolas Mansard, LAAS-CNRS
 *
 */

#ifndef __sot_core_entity_helper_H__
#define __sot_core_entity_helper_H__


namespace dynamicgraph
{

  template< typename Ent >
    struct EntityHelper
    {
      typedef Ent EntityClassName;
      //static const std::string CLASS_NAME; TO BE ADDED IN DG DIRECTLY
    };

} // namespace dynamicgraph

#endif // __sot_core_entity_helper_H__
