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
  CustomEntity(const std::string n) : Entity(n) {
    logger_.setTimeSample(0.001);
    logger_.setStreamPrintPeriod(0.005);
    logger_.setVerbosity(VERBOSITY_ALL);
    LoggerVerbosity alv = logger_.getVerbosity();
    BOOST_CHECK(alv == VERBOSITY_ALL);
  }

  ~CustomEntity() {}
  void testDebugTrace() {
    sendMsg("This is a message of level MSG_TYPE_DEBUG", MSG_TYPE_DEBUG);
    sendMsg("This is a message of level MSG_TYPE_INFO", MSG_TYPE_INFO);
    sendMsg("This is a message of level MSG_TYPE_WARNING", MSG_TYPE_WARNING);
    sendMsg("This is a message of level MSG_TYPE_ERROR", MSG_TYPE_ERROR);
    sendMsg("This is a message of level MSG_TYPE_DEBUG_STREAM",
            MSG_TYPE_DEBUG_STREAM);
    sendMsg("This is a message of level MSG_TYPE_INFO_STREAM",
            MSG_TYPE_INFO_STREAM);
    sendMsg("This is a message of level MSG_TYPE_WARNING_STREAM",
            MSG_TYPE_WARNING_STREAM);
    sendMsg("This is a message of level MSG_TYPE_ERROR_STREAM",
            MSG_TYPE_ERROR_STREAM);
    /* Add test toString */
    double q=1.0;
    sendMsg("Value to display: "+toString(q));
    std::vector<double> vq;
    vq.resize(3);
    vq[0] = 1.0; vq[1] = 2.0; vq[2] = 3.0;
    sendMsg("Value to display: "+toString(vq));
    Eigen::Matrix<double,3,3> an_eig_m;
    an_eig_m.Ones();
    sendMsg("Value to display: "+toString(an_eig_m));
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
  entity.setStreamPrintPeriod(0.004);
  BOOST_CHECK_EQUAL(entity.getStreamPrintPeriod(), 0.004);

  for (unsigned int i = 0; i < 10000; i++) {
    entity.testDebugTrace();
  }

  dynamicgraph::RealTimeLogger::destroy();
}
