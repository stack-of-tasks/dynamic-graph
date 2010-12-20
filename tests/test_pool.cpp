/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
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


#include <string>
#include <iostream>
#include <cstdlib>

#include <dynamic-graph/factory.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/pool.h>

#include <memory>

using namespace std;
using namespace dynamicgraph;

class MyEntity
: public Entity
{

 public:
	MyEntity(const std::string& name);


 public: /* --- ENTITY INHERITANCE --- */
  static const std::string CLASS_NAME;
  virtual void display( std::ostream& os ) const;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }


 protected:
};

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(MyEntity,"MyEntity");

MyEntity::MyEntity(const std::string& name)
	: Entity(name){

}

void MyEntity::display(std::ostream& os ) const {
	os << "Hello! My name is " << getName() << " !" << endl;
}

int main() {
	MyEntity myEntity("MyEntity");

	cout << "-- Pool.list" << endl;
	g_pool.commandLine("pool", "list", *auto_ptr<istringstream>(new istringstream("")), cout);
	Entity& e = g_pool.getEntity("MyEntity");
	cout << "-- Display" << endl;
	e.display(cout);
}
