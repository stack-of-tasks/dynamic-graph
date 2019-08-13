// Copyright 2010 Thomas Moulard.
//

#include <sstream>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>

#define BOOST_TEST_MODULE customEntity

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

struct CustomEntity : public dynamicgraph::Entity
{
  static const std::string CLASS_NAME;

  virtual const std::string& getClassName () const
  {
    return CLASS_NAME;
  }

  CustomEntity (const std::string n)
    : Entity (n)
  {
  }

  virtual ~CustomEntity()
  {
  }

  void display (std::ostream& os) const
  {
    os << "custom entity";
  }

};

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN (CustomEntity,"CustomEntity");


BOOST_AUTO_TEST_CASE (constructor)
{
  BOOST_CHECK_EQUAL (CustomEntity::CLASS_NAME, "CustomEntity");

  dynamicgraph::Entity* entity =
    dynamicgraph::FactoryStorage::getInstance()->
    newEntity("CustomEntity", "my-entity");
  BOOST_CHECK_EQUAL (entity->getName (), "my-entity");
  BOOST_CHECK_EQUAL (entity->Entity::getClassName (), "Entity");
  BOOST_CHECK_EQUAL (entity->getClassName (), CustomEntity::CLASS_NAME);

  //CustomEntity entity2 ("");
  // Deregister entities before destroying them
  dynamicgraph::PoolStorage::destroy();
}

BOOST_AUTO_TEST_CASE (display)
{
  dynamicgraph::Entity* entity = dynamicgraph::FactoryStorage::getInstance()->
    newEntity("CustomEntity", "my-entity");

  output_test_stream output;

  entity->display(output);
  BOOST_CHECK (output.is_equal ("custom entity"));
  // Deregister entities before destroying them
  dynamicgraph::PoolStorage::destroy();
}
