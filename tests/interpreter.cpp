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
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/interpreter.h>

#define BOOST_TEST_MODULE interpreter

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

// This unit test checks the dynamic-graph interpreter
// and its related mechanisms.
//
// It may be split in several tests in the future if this
// becomes too big.


// Simple test to check the shell default constructor.
BOOST_AUTO_TEST_CASE (constructor)
{
  dynamicgraph::Interpreter shell;
}


// Check that an empty command is valid.
BOOST_AUTO_TEST_CASE (cmd_empty)
{
  output_test_stream output;
  std::istringstream args;
  dynamicgraph::Interpreter shell;
  shell.cmd ("", args, output);
  BOOST_CHECK (output.is_empty ());
}


// Check that a non existing command returns
// the appropriate error.
BOOST_AUTO_TEST_CASE (cmd_notexist)
{
  output_test_stream output;
  std::istringstream args;
  dynamicgraph::Interpreter shell;
  try
    {
      shell.cmd ("I DO NOT EXIST", args, output);
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::UNREFERED_FUNCTION);
    }
}


// Check that the "help" command works.
BOOST_AUTO_TEST_CASE (cmd_help)
{
  output_test_stream output;
  std::istringstream args;
  dynamicgraph::Interpreter shell;
  shell.cmd ("help", args, output);
  BOOST_CHECK
    (output.is_equal
     (
      "Help\n"
      "  - compute <obj.sig> <time>			Recompute <sig> at time <time>  \n"
      "  - destroy <object>			Destroy an object.\n"
      "  - get <obj.signal> <value>			Get the signal to the constant value.\n"
      "  - import <script.txt>				Import the script.\n"
      "				Behaves like run but searches for files in default script directories.\n"
      "  - loadPlugin <file.so> <directory>		Load the plugin from the specified directory.\n"
      "  - new <class> <object>			Create a new entity.\n"
      "  - plug <obj1.sig1> <obj2.sig2>		Plug sig1 (producer) on sig2 (consumer).\n"
      "  - popImportPaths <path>				Drop path from default directories.\n"
      "  - pushImportPaths <path>				Add path to default directories.\n"
      "  - run <script.txt>				Run the script.\n"
      "  - set <obj.signal> <value>			Set the signal to the constant value.\n"
      "  - unloadPlugin <path/file.so>		Unload the plugin.\n"
      ));
}


// Check that an empty file can be parsed successfully.
BOOST_AUTO_TEST_CASE (cmd_run_emptyfile)
{
  output_test_stream output;
  dynamicgraph::Interpreter shell;

  std::stringstream ss;

  std::string cmd = "run";
  ss << TESTS_DATADIR "/empty.sot";

  std::istringstream args (ss.str ());
  shell.cmd (cmd, args, output);
  BOOST_CHECK (output.is_empty ());
}

// Check that trying to run a non existing file returns an appropriate
// error.
BOOST_AUTO_TEST_CASE (cmd_run_notexist)
{
  output_test_stream output;
  dynamicgraph::Interpreter shell;

  std::stringstream ss;

  std::string cmd = "run";
  ss << "idonotexist";

  std::istringstream args (ss.str ());
  try
    {
      shell.cmd (cmd, args, output);
      BOOST_ERROR ("Should never happen");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::READ_FILE);
    }
}

// Check that import a non existing file returns an appropriate error.
BOOST_AUTO_TEST_CASE (cmd_import_notexist)
{
  output_test_stream output;
  dynamicgraph::Interpreter shell;

  std::stringstream ss;

  std::string cmd = "import";
  ss << "idonotexist";

  std::istringstream args (ss.str ());
  try
    {
      shell.cmd (cmd, args, output);
      BOOST_ERROR ("Should never happen");
    }
  catch (const dynamicgraph::ExceptionFactory& exception)
    {
      BOOST_CHECK_EQUAL (exception.getCode (),
			 dynamicgraph::ExceptionFactory::READ_FILE);
    }
}

// Check that import/pushImportPaths/popImportPaths works.
BOOST_AUTO_TEST_CASE (cmd_import_push)
{
  dynamicgraph::Interpreter shell;

  // Push path.
  {
    output_test_stream output;
    std::stringstream ss;
    
    std::string cmd = "pushImportPaths";
    ss << TESTS_DATADIR;
    std::istringstream args (ss.str ());
    shell.cmd (cmd, args, output);
    BOOST_CHECK (output.is_empty ());
  }

  // Import empty file.
  {
    output_test_stream output;
    std::stringstream ss;
    
    std::string cmd = "import";
    ss << "empty.sot";
    std::istringstream args (ss.str ());
    shell.cmd (cmd, args, output);
    BOOST_CHECK (output.is_empty ());
  }

  // Pop path.
  {
    output_test_stream output;
    std::stringstream ss;
    
    std::string cmd = "popImportPaths";
    std::istringstream args (ss.str ());
    shell.cmd (cmd, args, output);
    BOOST_CHECK (output.is_empty ());
  }

  // Import empty file again: this will fail
  // as empty.sot is *NOT* in a standard location.
  {
    output_test_stream output;
    std::stringstream ss;
    
    std::string cmd = "import";
    ss << "empty.sot";
    std::istringstream args (ss.str ());
    
    // Make sure this trigger an error.
    BOOST_CHECK_THROW (shell.cmd (cmd, args, output),
		       dynamicgraph::ExceptionFactory);
  }

}
