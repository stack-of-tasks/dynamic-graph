// Copyright 2010 Thomas Moulard.
//

#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/plugin-loader.h>
#include <sstream>

#define BOOST_TEST_MODULE tracer

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

#include "interpreter.h"

using boost::test_tools::output_test_stream;

// Check that plug-in loading/unloading is working.
BOOST_AUTO_TEST_CASE(cmd_tracer) {
  dynamicgraph::PluginLoader pl;

  // Push paths.
  {
    RUN_COMMAND("pushImportPaths", TESTS_DATADIR);
    BOOST_CHECK(output.is_empty());
  }
  {
    RUN_COMMAND("pushImportPaths", TESTS_PLUGINDIR);
    BOOST_CHECK(output.is_empty());
  }

  // Import tracer.dg
  {
    RUN_COMMAND("import", "interpreter-tracer.dg");
    BOOST_CHECK(output.is_empty());
  }
}
