/* Copyright 2019, LAAS-CNRS
 *
 * Olivier Stasse
 *
 */
#include "dynamic-graph/factory.h"
#include "dynamic-graph/pool.h"
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <iostream>
#include <sstream>

#define VP_DEBUG 1
#define VP_DEBUG_MODE 50
#define VP_TEMPLATE_DEBUG_MODE 50

#include <dynamic-graph/debug.h>

#define BOOST_TEST_MODULE debug - trace

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

namespace dynamicgraph {
class CustomEntity : public Entity {
public:
  static const std::string CLASS_NAME;
  virtual const std::string &getClassName() const { return CLASS_NAME; }
  explicit CustomEntity(const std::string &n) : Entity(n) {
    dynamicgraph::dgDEBUGFLOW.openFile("/tmp/dynamic-graph-traces.txt");
  }
  ~CustomEntity() {
    dynamicgraph::dgDEBUGFLOW.closeFile("/tmp/dynamic-graph-traces.txt");
  }
  void testDebugTrace() {
    /// Test debugging information when entering the code.
    dgDEBUGIN(5);

    /// Intermediate test.
    dgDEBUGINOUT(5);

    dgDEBUG(5) << "Here is a test" << std::endl;

    /// Test debugging information when going out of the code.
    dgDEBUGOUT(5);
  }
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(CustomEntity, "CustomEntity");
} // namespace dynamicgraph

BOOST_AUTO_TEST_CASE(testDebugTrace) {
  BOOST_CHECK_EQUAL(dynamicgraph::CustomEntity::CLASS_NAME, "CustomEntity");

  dynamicgraph::CustomEntity *ptr_entity =
      (dynamic_cast<dynamicgraph::CustomEntity *>(
          dynamicgraph::FactoryStorage::getInstance()->newEntity("CustomEntity",

                                                                 "my-entity")));

  dynamicgraph::CustomEntity &entity = *ptr_entity;
  entity.testDebugTrace();

  /// Copy the debug file into the oss_debug_file
  output_test_stream output;
  std::fstream the_debug_file;
  the_debug_file.open(dynamicgraph::DebugTrace::DEBUG_FILENAME_DEFAULT,
                      std::ios::in);

  // Extract the filename and this source file from the output
  std::string astr;
  std::ostringstream oss_debug_file;
  while (std::getline(the_debug_file, astr)) {
    std::size_t found = astr.find(":");
    std::string asubstr = astr.substr(found + 1, astr.length());
    found = asubstr.find(":");
    std::string asubstr2 = asubstr.substr(found + 1, astr.length());
    oss_debug_file << asubstr2;
  }

  the_debug_file.close();

  // Compare with the strings put inside this source file
  std::string str_to_test = "# In {"
                            "# In/Out { }"
                            "Here is a test"
                            "# Out }";
  bool two_sub_string_identical;

  // Make comparisons.
  two_sub_string_identical = str_to_test == oss_debug_file.str();

  BOOST_CHECK(two_sub_string_identical);

  delete ptr_entity;
}
