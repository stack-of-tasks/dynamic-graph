// Copyright 2010 Thomas Moulard.
//

#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/factory.h>
#include <sstream>

#define BOOST_TEST_MODULE factory

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

using boost::test_tools::output_test_stream;

namespace dynamicgraph {
class CustomEntity : public Entity {
public:
  static const std::string CLASS_NAME;
  virtual const std::string &getClassName() const { return CLASS_NAME; }
  CustomEntity(const std::string n) : Entity(n) {}
};
const std::string CustomEntity::CLASS_NAME = "CustomEntity";
} // namespace dynamicgraph

dynamicgraph::Entity *makeEntity(const std::string &objectName) {
  return new dynamicgraph::CustomEntity(objectName);
}

BOOST_AUTO_TEST_CASE(constructor) {
  dynamicgraph::FactoryStorage::getInstance();
}

BOOST_AUTO_TEST_CASE(registerEntity) {
  dynamicgraph::FactoryStorage::getInstance()->registerEntity("myEntity",
                                                              &makeEntity);

  try {
    dynamicgraph::FactoryStorage::getInstance()->registerEntity("myEntity",
                                                                &makeEntity);
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
  }

  try {
    dynamicgraph::FactoryStorage::getInstance()->registerEntity("myEntity", 0);
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
  }
}

BOOST_AUTO_TEST_CASE(unregisterEntity) {
  dynamicgraph::FactoryStorage::getInstance()->deregisterEntity("myEntity");

  try {
    dynamicgraph::FactoryStorage::getInstance()->deregisterEntity("myEntity");
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
  }

  try {
    dynamicgraph::FactoryStorage::getInstance()->deregisterEntity(
        "I do not exist.");
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::OBJECT_CONFLICT);
  }
}

BOOST_AUTO_TEST_CASE(newEntity) {
  dynamicgraph::FactoryStorage::getInstance()->registerEntity("myEntity",
                                                              &makeEntity);

  {
    boost::shared_ptr<dynamicgraph::Entity> entity(
        dynamicgraph::FactoryStorage::getInstance()->newEntity("myEntity",
                                                               "foo"));

    boost::shared_ptr<dynamicgraph::Entity> entity2(
        dynamicgraph::FactoryStorage::getInstance()->newEntity("myEntity",
                                                               "foo2"));

    boost::shared_ptr<dynamicgraph::Entity> entity3(
        dynamicgraph::FactoryStorage::getInstance()->newEntity("myEntity", ""));
  }

  try {
    dynamicgraph::FactoryStorage::getInstance()->newEntity("I do not exist.",
                                                           "");
    BOOST_ERROR("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::UNREFERED_OBJECT);
  }

  try {
    dynamicgraph::FactoryStorage::getInstance()->destroy();
    dynamicgraph::FactoryStorage::getInstance()->existEntity("myEntity");
    // BOOST_ERROR ("Should never happen.");
  } catch (const dynamicgraph::ExceptionFactory &exception) {
    BOOST_CHECK_EQUAL(exception.getCode(),
                      dynamicgraph::ExceptionFactory::UNREFERED_OBJECT);
  }
}

BOOST_AUTO_TEST_CASE(existEntity) {
  // BOOST_CHECK (dynamicgraph::FactoryStorage::getInstance()->existEntity
  //        ("myEntity"));
  BOOST_CHECK(
      !dynamicgraph::FactoryStorage::getInstance()->existEntity("myEntity2"));
  BOOST_CHECK(!dynamicgraph::FactoryStorage::getInstance()->existEntity(""));
}
