/*
 * Copyright 2011, Nicolas Mansard, LAAS-CNRS
 *
 * This file is part of sot-core.
 * sot-core is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * sot-core is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with sot-core.  If not, see <http://www.gnu.org/licenses/>.
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
