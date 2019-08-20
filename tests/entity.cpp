/* Copyright 2010-2019 LAAS, CNRS
 * Thomas Moulard.
 *
 */

#define ENABLE_RT_LOG

#include "dynamic-graph/factory.h"
#include "dynamic-graph/pool.h"
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/real-time-logger.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <sstream>

#define BOOST_TEST_MODULE entity

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

namespace dynamicgraph {
class CustomEntity : public Entity {
public:
  dynamicgraph::SignalPtr<double, int> m_sigdSIN;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTimeDepSOUT;

  static const std::string CLASS_NAME;
  virtual const std::string &getClassName() const { return CLASS_NAME; }
  CustomEntity(const std::string n)
      : Entity(n),
        m_sigdSIN(NULL, "CustomEntity(" + name + ")::input(double)::in_double"),
        m_sigdTimeDepSOUT(
            boost::bind(&CustomEntity::update, this, _1, _2), m_sigdSIN,
            "CustomEntity(" + name + ")::input(double)::out_double")

  {}

  void addSignal() { signalRegistration(m_sigdSIN << m_sigdTimeDepSOUT); }

  void rmValidSignal() {
    signalDeregistration("in_double");
    signalDeregistration("out_double");
  }

  double &update(double &res, const int &inTime) {
    const double &aDouble = m_sigdSIN(inTime);
    res = aDouble;
    return res;
  }
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(CustomEntity, "CustomEntity");
} // namespace dynamicgraph

BOOST_AUTO_TEST_CASE(constructor) {
  BOOST_CHECK_EQUAL(dynamicgraph::CustomEntity::CLASS_NAME, "CustomEntity");

  dynamicgraph::Entity &entity =
      *dynamicgraph::FactoryStorage::getInstance()->newEntity("CustomEntity",
                                                              "my-entity");
  BOOST_CHECK_EQUAL(entity.getName(), "my-entity");
  BOOST_CHECK_EQUAL(entity.getClassName(),
                    dynamicgraph::CustomEntity::CLASS_NAME);

  dynamicgraph::CustomEntity entity2("");
}

BOOST_AUTO_TEST_CASE(signal) {
  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  // Non const getter.
  try {
    entity.getSignal("I do not exist");
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::UNREFERED_SIGNAL);
  }

  // Const getter.
  try {
    const dynamicgraph::Entity &entityConst = entity;
    entityConst.getSignal("I do not exist");
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::UNREFERED_SIGNAL);
  }
  // deregistration
  try {
    dynamicgraph::CustomEntity *customEntity =
        dynamic_cast<dynamicgraph::CustomEntity *>(&entity);
    customEntity->addSignal();
    // Removing signals is working the first time
    customEntity->rmValidSignal();
    // Removing signals generates an exception the second time.
    customEntity->rmValidSignal();
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::UNREFERED_SIGNAL);
  }
}

BOOST_AUTO_TEST_CASE(displaySignalList) {
  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;

  entity.displaySignalList(output);
  BOOST_CHECK(output.is_equal("--- <my-entity> signal list: \n"));
}

BOOST_AUTO_TEST_CASE(display) {
  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;

  entity.display(output);
  BOOST_CHECK(output.is_equal("CustomEntity: my-entity"));
}

BOOST_AUTO_TEST_CASE(getCommandList) {
  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  BOOST_CHECK_EQUAL(entity.getCommandList(), "print\nsignals\nsignalDep");
}

BOOST_AUTO_TEST_CASE(writeGraph) {
  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;
  entity.writeGraph(output);

  BOOST_CHECK(output.is_equal(""));
}

BOOST_AUTO_TEST_CASE(writeCompletionList) {
  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;
  entity.writeCompletionList(output);

  BOOST_CHECK(output.is_equal("print\nsignals\nsignalDep\n"));
}

BOOST_AUTO_TEST_CASE(sendMsg) {
  std::ofstream of;
  of.open("/tmp/dg-LOGS.txt", std::ofstream::out | std::ofstream::app);
  dgADD_OSTREAM_TO_RTLOG(of);

  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  output_test_stream output;

  for (unsigned int i = 0; i < 4; i++) {
    for (unsigned int j = 0; j < 2000; j++) {
      dynamicgraph::LoggerVerbosity aLoggerVerbosityLevel =
          (dynamicgraph::LoggerVerbosity)i;
      entity.setLoggerVerbosityLevel(aLoggerVerbosityLevel);
      if (entity.getLoggerVerbosityLevel() != aLoggerVerbosityLevel)
        output << "Mismatch output";

      std::string aBaseMsg = "Auto Test Case";
      std::string aMsg = aBaseMsg + " DEBUG";
      entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_DEBUG, __FILE__, __LINE__);
      aMsg = aBaseMsg + " INFO";
      entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_INFO, __FILE__, __LINE__);
      aMsg = aBaseMsg + " WARNING";
      entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_WARNING, __FILE__, __LINE__);
      aMsg = aBaseMsg + " DEBUG";
      entity.sendMsg(aMsg, dynamicgraph::MSG_TYPE_ERROR, __FILE__, __LINE__);
    };
  };

  BOOST_CHECK(output.is_equal(""));
  dynamicgraph::RealTimeLogger::destroy();
}

// WTF?
BOOST_AUTO_TEST_CASE(wtf) {
  dynamicgraph::Entity &entity =
      dynamicgraph::PoolStorage::getInstance()->getEntity("my-entity");

  BOOST_CHECK_EQUAL(entity.test(),
                    static_cast<dynamicgraph::SignalBase<int> *>(0));

  entity.test2(static_cast<dynamicgraph::SignalBase<int> *>(0));
}
