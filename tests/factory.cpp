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
#include <dynamic-graph/factory.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>

#define BOOST_TEST_MODULE factory

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;


dynamicgraph::Entity* makeEntity(const std::string& objectName)
{
  return new dynamicgraph::Entity (objectName);
}


BOOST_AUTO_TEST_CASE (constructor)
{
  dynamicgraph::FactoryStorage::getInstance();
}

BOOST_AUTO_TEST_CASE (registerEntity)
{
  dynamicgraph::FactoryStorage::getInstance()->registerEntity
    ("myEntity", &makeEntity);

  try
    {
      dynamicgraph::FactoryStorage::getInstance()->registerEntity
	("myEntity", &makeEntity);
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
    }

  try
    {
      dynamicgraph::FactoryStorage::getInstance()->registerEntity
	("myEntity", 0);
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
    }

}

BOOST_AUTO_TEST_CASE (unregisterEntity)
{
  dynamicgraph::FactoryStorage::getInstance()->registerEntity ("myEntity", &makeEntity);
  dynamicgraph::FactoryStorage::getInstance()->deregisterEntity ("myEntity");

  try
    {
      dynamicgraph::FactoryStorage::getInstance()->deregisterEntity("myEntity");
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
    }

  try
    {
      dynamicgraph::FactoryStorage::getInstance()->deregisterEntity
	("I do not exist.");
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
    }
}

BOOST_AUTO_TEST_CASE (newEntity)
{
  dynamicgraph::FactoryStorage::getInstance()->registerEntity
    ("myEntity", &makeEntity);

  {
    boost::shared_ptr<dynamicgraph::Entity> entity
      (dynamicgraph::FactoryStorage::getInstance()->newEntity
       ("myEntity", "foo"));

    boost::shared_ptr<dynamicgraph::Entity> entity2
      (dynamicgraph::FactoryStorage::getInstance()->newEntity
       ("myEntity", "foo2"));

    boost::shared_ptr<dynamicgraph::Entity> entity3
      (dynamicgraph::FactoryStorage::getInstance()->newEntity
       ("myEntity", ""));
  }

  try
    {
      dynamicgraph::FactoryStorage::getInstance()->newEntity
	("I do not exist.", "");
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::UNREFERED_OBJECT);
    }
}

BOOST_AUTO_TEST_CASE (existEntity)
{
  dynamicgraph::FactoryStorage::getInstance()->registerEntity
    ("myEntity", &makeEntity);

  BOOST_CHECK (dynamicgraph::FactoryStorage::getInstance()->existEntity
	       ("myEntity"));
  BOOST_CHECK (!dynamicgraph::FactoryStorage::getInstance()->existEntity
	       ("myEntity2"));
  BOOST_CHECK (!dynamicgraph::FactoryStorage::getInstance()->existEntity (""));
}
