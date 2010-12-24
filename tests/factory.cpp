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
  dynamicgraph::FactoryStorage factory;
}

BOOST_AUTO_TEST_CASE (registerEntity)
{
  dynamicgraph::FactoryStorage factory;
  factory.registerEntity ("myEntity", &makeEntity);

  try
    {
      factory.registerEntity ("myEntity", &makeEntity);
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
    }

  try
    {
      factory.registerEntity ("myEntity", 0);
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
  dynamicgraph::FactoryStorage factory;
  factory.registerEntity ("myEntity", &makeEntity);
  factory.deregisterEntity ("myEntity");

  try
    {
      factory.deregisterEntity ("myEntity");
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
    }

  try
    {
      factory.deregisterEntity ("I do not exist.");
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
  dynamicgraph::FactoryStorage factory;
  factory.registerEntity ("myEntity", &makeEntity);

  {
    boost::shared_ptr<dynamicgraph::Entity> entity
      (factory.newEntity ("myEntity", "foo"));

    boost::shared_ptr<dynamicgraph::Entity> entity2
      (factory.newEntity ("myEntity", "foo"));

    boost::shared_ptr<dynamicgraph::Entity> entity3
      (factory.newEntity ("myEntity", ""));
  }

  try
    {
      factory.newEntity ("I do not exist.", "");
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
    }
}

BOOST_AUTO_TEST_CASE (existEntity)
{
  dynamicgraph::FactoryStorage factory;
  factory.registerEntity ("myEntity", &makeEntity);

  BOOST_CHECK (factory.existEntity ("myEntity"));
  BOOST_CHECK (!factory.existEntity ("myEntity2"));
  BOOST_CHECK (!factory.existEntity (""));
}
