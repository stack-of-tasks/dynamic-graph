/* Copyright 2019, LAAS-CNRS
 *
 * Olivier Stasse
 *
 * See LICENSE file
 *
 */
#include "dynamic-graph/factory.h"
#include "dynamic-graph/pool.h"
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <iostream>
#include <sstream>

#define ENABLE_RT_LOG
#include <dynamic-graph/logger.h>
#include <dynamic-graph/real-time-logger.h>

#define BOOST_TEST_MODULE debug - logger

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

namespace dynamicgraph {
class CustomEntity : public Entity {
public:
  static const std::string CLASS_NAME;
  virtual const std::string &getClassName() const { return CLASS_NAME; }
  explicit CustomEntity(const std::string &n) : Entity(n) {
    logger_.setTimeSample(0.001);
    logger_.setStreamPrintPeriod(0.005);

    logger_.setVerbosity(VERBOSITY_NONE);
    BOOST_CHECK_EQUAL(logger_.getVerbosity(), VERBOSITY_NONE);
    BOOST_CHECK( logger_.stream(MSG_TYPE_DEBUG  ).isNull());
    BOOST_CHECK( logger_.stream(MSG_TYPE_INFO   ).isNull());
    BOOST_CHECK( logger_.stream(MSG_TYPE_WARNING).isNull());
    BOOST_CHECK( logger_.stream(MSG_TYPE_ERROR  ).isNull());

    logger_.setVerbosity(VERBOSITY_ERROR);
    BOOST_CHECK_EQUAL(logger_.getVerbosity(), VERBOSITY_ERROR);
    BOOST_CHECK( logger_.stream(MSG_TYPE_DEBUG  ).isNull());
    BOOST_CHECK( logger_.stream(MSG_TYPE_INFO   ).isNull());
    BOOST_CHECK( logger_.stream(MSG_TYPE_WARNING).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_ERROR  ).isNull());

    logger_.setVerbosity(VERBOSITY_WARNING_ERROR);
    BOOST_CHECK_EQUAL(logger_.getVerbosity(), VERBOSITY_WARNING_ERROR);
    BOOST_CHECK( logger_.stream(MSG_TYPE_DEBUG  ).isNull());
    BOOST_CHECK( logger_.stream(MSG_TYPE_INFO   ).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_WARNING).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_ERROR  ).isNull());

    logger_.setVerbosity(VERBOSITY_INFO_WARNING_ERROR);
    BOOST_CHECK_EQUAL(logger_.getVerbosity(), VERBOSITY_INFO_WARNING_ERROR);
    BOOST_CHECK( logger_.stream(MSG_TYPE_DEBUG  ).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_INFO   ).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_WARNING).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_ERROR  ).isNull());

    logger_.setVerbosity(VERBOSITY_ALL);
    BOOST_CHECK_EQUAL(logger_.getVerbosity(), VERBOSITY_ALL);
    BOOST_CHECK(!logger_.stream(MSG_TYPE_DEBUG  ).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_INFO   ).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_WARNING).isNull());
    BOOST_CHECK(!logger_.stream(MSG_TYPE_ERROR  ).isNull());
  }

  ~CustomEntity() {}
  void testDebugTrace() {
    logger_.stream(MSG_TYPE_DEBUG)
      << "This is a message of level MSG_TYPE_DEBUG\n";
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_INFO)
      << "This is a message of level MSG_TYPE_INFO\n";
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_WARNING)
      << "This is a message of level MSG_TYPE_WARNING\n";
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_ERROR)
      << "This is a message of level MSG_TYPE_ERROR\n";
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_DEBUG_STREAM)
      << "This is a message of level MSG_TYPE_DEBUG_STREAM\n";
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_INFO_STREAM)
      << "This is a message of level MSG_TYPE_INFO_STREAM\n";
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_WARNING_STREAM)
      << "This is a message of level MSG_TYPE_WARNING_STREAM\n";
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_ERROR_STREAM)
      << "This is a message of level MSG_TYPE_ERROR_STREAM\n";
    /* Add test toString */
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    double q = 1.0;
    logger_.stream() << "Value to display: " + toString(q) << '\n';
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    std::vector<double> vq;
    vq.resize(3);
    vq[0] = 1.0;
    vq[1] = 2.0;
    vq[2] = 3.0;
    logger_.stream(MSG_TYPE_INFO) << "Value to display: " << toString(vq) << '\n';
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.stream(MSG_TYPE_INFO) << "Value to display: " << toString(vq, 3, 10) << '\n';
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    Eigen::Matrix<double, 3, 3> an_eig_m;
    an_eig_m.setOnes();
    logger_.stream(MSG_TYPE_INFO) << "Value to display: " << toString(an_eig_m) << '\n';
    dynamicgraph::RealTimeLogger::instance().spinOnce();
    logger_.countdown();
  }
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(CustomEntity, "CustomEntity");
} // namespace dynamicgraph

BOOST_AUTO_TEST_CASE(debug_logger) {
  std::ofstream of;
  dynamicgraph::RealTimeLogger::instance();
  of.open("/tmp/dg-LOGS.txt", std::ofstream::out | std::ofstream::app);
  dgADD_OSTREAM_TO_RTLOG(of);

  BOOST_CHECK_EQUAL(dynamicgraph::CustomEntity::CLASS_NAME, "CustomEntity");

  dynamicgraph::CustomEntity &entity =
      *(dynamic_cast<dynamicgraph::CustomEntity *>(
          dynamicgraph::FactoryStorage::getInstance()->newEntity("CustomEntity",
                                                                 "my-entity")));

  entity.setTimeSample(0.002);
  BOOST_CHECK_EQUAL(entity.getTimeSample(), 0.002);
  entity.setStreamPrintPeriod(0.002);
  BOOST_CHECK_EQUAL(entity.getStreamPrintPeriod(), 0.002);

  for (unsigned int i = 0; i < 10000; i++) {
    entity.testDebugTrace();
  }

  dynamicgraph::RealTimeLogger::destroy();
}
