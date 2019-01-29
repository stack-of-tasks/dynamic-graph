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

#define ENABLE_RT_LOG

#include <sstream>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include "dynamic-graph/factory.h"
#include "dynamic-graph/pool.h"
#include <dynamic-graph/real-time-logger.h>

#define BOOST_TEST_MODULE entity

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

namespace dynamicgraph
{
  class CustomEntity : public Entity
  {
  public:
    static const std::string CLASS_NAME;
    virtual const std::string& getClassName () const
    {
      return CLASS_NAME;
    }
    CustomEntity (const std::string n)
      : Entity (n)
    {
    }
  };
  DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN (CustomEntity,"CustomEntity");
}


BOOST_AUTO_TEST_CASE (constructor)
{
  BOOST_CHECK_EQUAL (dynamicgraph::CustomEntity::CLASS_NAME, "CustomEntity");

  dynamicgraph::Entity& entity =
    *dynamicgraph::FactoryStorage::getInstance()->newEntity("CustomEntity",
							    "my-entity");
  BOOST_CHECK_EQUAL (entity.getName (), "my-entity");
  BOOST_CHECK_EQUAL (entity.getClassName (),
		     dynamicgraph::CustomEntity::CLASS_NAME);

   dynamicgraph::CustomEntity entity2 ("");
}

BOOST_AUTO_TEST_CASE (signal)
{
  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  // Non const getter.
  try
    {
      entity.getSignal ("I do not exist");
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::UNREFERED_SIGNAL);
    }

  // Const getter.
  try
    {
      const dynamicgraph::Entity& entityConst = entity;
      entityConst.getSignal ("I do not exist");
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::UNREFERED_SIGNAL);
    }
}

BOOST_AUTO_TEST_CASE (displaySignalList)
{
  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;

  entity.displaySignalList(output);
  BOOST_CHECK (output.is_equal ("--- <my-entity> signal list: \n"));
}

BOOST_AUTO_TEST_CASE (display)
{
  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;

  entity.display(output);
  BOOST_CHECK (output.is_equal ("CustomEntity: my-entity"));
}

BOOST_AUTO_TEST_CASE (getCommandList)
{
  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  BOOST_CHECK_EQUAL (entity.getCommandList (), "print\nsignals\nsignalDep");
}

BOOST_AUTO_TEST_CASE (writeGraph)
{
  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;
  entity.writeGraph (output);

  BOOST_CHECK (output.is_equal (""));
}

BOOST_AUTO_TEST_CASE (writeCompletionList)
{
  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;
  entity.writeGraph (output);

  BOOST_CHECK (output.is_equal (""));
}

BOOST_AUTO_TEST_CASE (sendMsg)
{
  std::ofstream of;
  of.open("/tmp/dg-LOGS.txt",std::ofstream::out|std::ofstream::app);
  dgADD_OSTREAM_TO_RTLOG(of);

  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");
  std::string AppendMsg[4] = {" INFO_WARNING_ERROR",
			      " WARNING_ERROR",
			      " ERROR",
			      " ALL",
  };

  output_test_stream output;
  
  for(unsigned int i=0;
      i<4;
      i++)
    {
      for(unsigned int j=0;j<2000;j++)
	{
	  dynamicgraph::LoggerVerbosity aLoggerVerbosityLevel=
	    (dynamicgraph::LoggerVerbosity) i;
	  entity.setLoggerVerbosityLevel(aLoggerVerbosityLevel);
	  if (entity.getLoggerVerbosityLevel()!=aLoggerVerbosityLevel)
	    output << "Mismatch output";
	  
	  std::string aBaseMsg="Auto Test Case";
	  std::string aMsg=aBaseMsg+" DEBUG";
	  entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_DEBUG, __FILE__, __LINE__);
	  aMsg=aBaseMsg+" INFO";
	  entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_INFO, __FILE__, __LINE__);
	  aMsg=aBaseMsg+" WARNING";
	  entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_WARNING, __FILE__, __LINE__);
	  aMsg=aBaseMsg+" DEBUG";
	  entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_ERROR, __FILE__, __LINE__);
	};
    };

  BOOST_CHECK (output.is_equal (""));
  usleep (1000000);
  dynamicgraph::RealTimeLogger::destroy();
}

// WTF?
BOOST_AUTO_TEST_CASE (wtf)
{
  dynamicgraph::Entity& entity =
    dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  BOOST_CHECK_EQUAL (entity.test (),
		     static_cast<dynamicgraph::SignalBase<int>*> (0));

  entity.test2 (static_cast<dynamicgraph::SignalBase<int>*> (0));
}
