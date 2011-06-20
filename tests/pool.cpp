// Copyright 2010 Thomas Moulard.
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

#include <sstream>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/pool.h>

#define BOOST_TEST_MODULE pool

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

struct MyEntity : public dynamicgraph::Entity
{
  static const std::string CLASS_NAME;

  MyEntity (const std::string& name)
    : Entity (name)
  {}

  virtual void display (std::ostream& os) const
  {
    os << "Hello! My name is " << getName () << " !" << std::endl;
  }

  virtual const std::string& getClassName () const
  {
    return CLASS_NAME;
  }
};

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN (MyEntity, "MyEntity");

BOOST_AUTO_TEST_CASE (pool_list)
{
  MyEntity myEntity("MyEntityInst");
  std::istringstream in;
  output_test_stream output;
  dynamicgraph::PoolStorage::getInstance()->commandLine
    ("pool", "list", in, output);
  BOOST_CHECK (output.is_equal ("MyEntityInst (MyEntity)\n"));
  dynamicgraph::PoolStorage::getInstance()->deregisterEntity
    (myEntity.getName());
}

BOOST_AUTO_TEST_CASE (pool_display)
{
  MyEntity myEntity("MyEntityInst");
  output_test_stream output;
  dynamicgraph::Entity& e = dynamicgraph::PoolStorage::getInstance()->getEntity
    ("MyEntityInst");
  e.display(output);
  BOOST_CHECK (output.is_equal ("Hello! My name is MyEntityInst !\n"));
  dynamicgraph::PoolStorage::getInstance()->deregisterEntity
    (myEntity.getName());
}
