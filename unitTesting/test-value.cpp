/*
 * Copyright 2010,
 * Florent Lamiraux
 *
 * CNRS/LAAS
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

/* -------------------------------------------------------------------------- */
/* --- INCLUDES ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#include <iostream>
#include "dynamic-graph/value.h"

int main()
{
  using dynamicgraph::command::Value;

  std::string str1("value #1");
  Value value1(str1);
  Value value = value1;

  std::cout << "value1 = " << value1 << std::endl;
  std::cout << "value = " << value << std::endl;

  std::string str2("value #2");
  Value value2(str2);
  value = value2;

  std::cout << "value2 = " << value2 << std::endl;
  std::cout << "value = " << value << std::endl;
  
}
