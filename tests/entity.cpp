/* Copyright 2010-2019 LAAS, CNRS
 * Thomas Moulard.
 *
 */

#define ENABLE_RT_LOG

#include "dynamic-graph/factory.h"
#include "dynamic-graph/pool.h"
#include <dynamic-graph/command-direct-getter.h>
#include <dynamic-graph/command-direct-setter.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/real-time-logger.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <sstream>

#define BOOST_TEST_MODULE entity

#if BOOST_VERSION >= 105900
#include <boost/test/tools/output_test_stream.hpp>
#else
#include <boost/test/output_test_stream.hpp>
#endif
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

namespace dynamicgraph {
class CustomEntity : public Entity {
public:
  dynamicgraph::SignalPtr<double, int> m_sigdSIN, m_sigdSIN2;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTimeDepSOUT;

  static const std::string CLASS_NAME;
  virtual const std::string &getClassName() const { return CLASS_NAME; }
  explicit CustomEntity(const std::string &n)
      : Entity(n),
        m_sigdSIN(NULL, "CustomEntity(" + name + ")::input(double)::in_double"),
        m_sigdSIN2(NULL,
                   "CustomEntity(" + name + ")::input(double)::in_double"),
        m_sigdTimeDepSOUT(
            boost::bind(&CustomEntity::update, this, boost::placeholders::_1,
                        boost::placeholders::_2),
            m_sigdSIN, "CustomEntity(" + name + ")::input(double)::out_double"),
        m_value(0.0) {}

  ~CustomEntity() { entityDeregistration(); }

  void addSignal() {
    signalRegistration(m_sigdSIN << m_sigdTimeDepSOUT);
    /// Try a second time to generate an exception
    try {
      signalRegistration(m_sigdSIN2 << m_sigdTimeDepSOUT);
    } catch (ExceptionFactory &aef) {
      BOOST_CHECK_EQUAL(aef.getCode(),
                        dynamicgraph::ExceptionFactory::SIGNAL_CONFLICT);
    }
  }

  void rmValidSignal() {
    signalDeregistration("in_double");
    signalDeregistration("out_double");
  }

  double &update(double &res, const int &inTime) {
    const double &aDouble = m_sigdSIN(inTime);
    res = aDouble;
    return res;
  }

public:
  double m_value;
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(CustomEntity, "CustomEntity");
} // namespace dynamicgraph

BOOST_AUTO_TEST_CASE(constructor) {
  namespace dg = dynamicgraph;
  namespace dgc = dynamicgraph::command;

  BOOST_CHECK_EQUAL(dg::CustomEntity::CLASS_NAME, "CustomEntity");

  dg::Entity &entity = *dg::FactoryStorage::getInstance()->newEntity(
      "CustomEntity", "my-entity");
  BOOST_CHECK_EQUAL(entity.getName(), "my-entity");
  BOOST_CHECK_EQUAL(entity.getClassName(), dg::CustomEntity::CLASS_NAME);

  dg::CustomEntity entity2("");

  // Test Commands
  dgc::DirectGetter<dg::CustomEntity, double> a_direct_getter(
      entity2, &entity2.m_value,
      dgc::docDirectGetter("Get value m_value", "double"));

  dgc::DirectSetter<dg::CustomEntity, double> a_direct_setter(
      entity2, &entity2.m_value,
      dgc::docDirectSetter("Set value m_value", "double"));

  dgc::Value aValue(2.0);
  std::vector<dgc::Value> values;
  values.push_back(aValue);
  a_direct_setter.setParameterValues(values);
  a_direct_setter.execute();
  a_direct_getter.execute();

  output_test_stream output;
  output << entity2.m_value;
  output << entity2;

  entity.getDocString();
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
    std::string signame("CustomEntity(my-entity)::input(double)::in_double");
    customEntity->Entity::hasSignal(signame);
    output_test_stream output;
    customEntity->Entity::displaySignalList(output);
    dynamicgraph::Entity::SignalMap asigmap = customEntity->getSignalMap();
    output << customEntity;
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

#define __FILELINE__ __FILE__ BOOST_PP_STRINGIZE(__LINE__)
      entity.logger().stream(dynamicgraph::MSG_TYPE_DEBUG, __FILELINE__)
          << "Auto Test Case"
          << " DEBUG" << '\n';
      entity.logger().stream(dynamicgraph::MSG_TYPE_INFO, __FILELINE__)
          << "Auto Test Case"
          << " INFO" << '\n';
      entity.logger().stream(dynamicgraph::MSG_TYPE_WARNING, __FILELINE__)
          << "Auto Test Case"
          << " WARNING" << '\n';
      entity.logger().stream(dynamicgraph::MSG_TYPE_ERROR, __FILELINE__)
          << "Auto Test Case"
          << " ERROR" << '\n';
#undef __FILELINE__
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
