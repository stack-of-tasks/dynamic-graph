/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-JAPAN, Tsukuba, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      test_entity.cc
 * Project:   dynamicGraph
 * Author:    François Bleibel
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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

main() {
	MyEntity myEntity("MyEntity");

	cout << "-- Pool.list" << endl;
	g_pool.commandLine("pool", "list", *auto_ptr<istringstream>(new istringstream("")), cout);
	Entity& e = g_pool.getEntity("MyEntity");
	cout << "-- Display" << endl;
	e.display(cout);
}
