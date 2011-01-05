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
#include <dynamic-graph/plugin-loader.h>

#define BOOST_TEST_MODULE tracer

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "interpreter.h"

using boost::test_tools::output_test_stream;

// Check that plug-in loading/unloading is working.
BOOST_AUTO_TEST_CASE (cmd_tracer)
{
  dynamicgraph::PluginLoader pl;
  dynamicgraph::Interpreter shell (&pl);

  // Push paths.
  {
    RUN_COMMAND ("pushImportPaths", TESTS_DATADIR);
    BOOST_CHECK (output.is_empty ());
  }
  {
    RUN_COMMAND ("pushImportPaths", TESTS_PLUGINDIR);
    BOOST_CHECK (output.is_empty ());
  }

  // Import tracer.dg
  {
    RUN_COMMAND ("import", "interpreter-tracer.dg");
    BOOST_CHECK (output.is_empty ());
  }
}
