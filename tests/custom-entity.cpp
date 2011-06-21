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

  virtual void commandLine (const std::string& cmdLine,
			    std::istringstream& cmdArgs,
			    std::ostream& os)
  {
    if (cmdLine == "help")
      {
	os << "This is a custom help." << std::endl;
	Entity::commandLine (cmdLine, cmdArgs, os);
      }
    else if (cmdLine == "nothing")
      ;
    else
      Entity::commandLine (cmdLine, cmdArgs, os);
  }
};

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN (CustomEntity,"CustomEntity");


BOOST_AUTO_TEST_CASE (constructor)
{
  BOOST_CHECK_EQUAL (CustomEntity::CLASS_NAME, "CustomEntity");

  CustomEntity entity ("my-entity");
  BOOST_CHECK_EQUAL (entity.getName (), "my-entity");
  BOOST_CHECK_EQUAL (entity.getClassName (), CustomEntity::CLASS_NAME);

  CustomEntity entity2 ("");
}

BOOST_AUTO_TEST_CASE (display)
{
  CustomEntity entity ("my-entity");

  output_test_stream output;

  entity.display(output);
  BOOST_CHECK (output.is_equal ("custom entity"));
}

BOOST_AUTO_TEST_CASE (commandLine_help)
{
  CustomEntity entity ("my-entity");

  output_test_stream output;

  std::istringstream args;

  entity.commandLine("help", args, output);
  BOOST_CHECK
    (output.is_equal
     (
      "This is a custom help.\n"
      "Entity : \n"
      "  - print\t\t\tWhat d'you think?\n"
      "  - signals\t\t\tDisplay the signals list.\n"
      "  - signalDep <signame> \tDisplay the dependency graph for signal signame.\n"
      ));
}

BOOST_AUTO_TEST_CASE (commandLine_print)
{
  CustomEntity entity ("my-entity");

  output_test_stream output;

  std::istringstream args;

  entity.commandLine("print", args, output);
  BOOST_CHECK (output.is_equal ("custom entity\n"));
}

BOOST_AUTO_TEST_CASE (commandLine_nothing)
{
  CustomEntity entity ("my-entity");

  output_test_stream output;

  std::istringstream args;

  entity.commandLine("nothing", args, output);
  BOOST_CHECK (output.is_empty ());
}

BOOST_AUTO_TEST_CASE (commandLine_unknown)
{
  CustomEntity entity ("my-entity");

  output_test_stream output;

  std::istringstream args;

  try
    {
      entity.commandLine("unknown", args, output);
      BOOST_ERROR ("Should never happen.");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::UNREFERED_FUNCTION);
    }

  BOOST_CHECK (output.is_empty ());
}
