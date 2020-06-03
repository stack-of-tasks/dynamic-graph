/* Copyright 2019, LAAS-CNRS
 *
 * Olivier Stasse
 *
 */

#include <iostream>

#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/tracer.h>

#define BOOST_TEST_MODULE debug - tracer

#include <boost/test/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>

namespace dynamicgraph {
struct MyEntity : public dynamicgraph::Entity {
  static const std::string CLASS_NAME;

  dynamicgraph::Signal<double, int> m_sigdSIN;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTimeDepSOUT;
  dynamicgraph::SignalTimeDependent<Vector, int> m_sigVTimeDepSOUT;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTwoTimeDepSOUT;

  explicit MyEntity(const std::string &name)
      : Entity(name),
        m_sigdSIN("MyEntity(" + name + ")::input(double)::in_double"),
        m_sigdTimeDepSOUT(boost::bind(&MyEntity::update, this, _1, _2),
                          m_sigdSIN,
                          "MyEntity(" + name + ")::input(double)::out_double"),
        m_sigVTimeDepSOUT(boost::bind(&MyEntity::updateVector, this, _1, _2),
                          m_sigdSIN,
                          "MyEntity(" + name + ")::input(vector)::out_vector"),
        m_sigdTwoTimeDepSOUT(
            boost::bind(&MyEntity::update, this, _1, _2), m_sigdSIN,
            "MyEntity(" + name + ")::input(double)::out2double")

  {
    signalRegistration(m_sigdSIN << m_sigdTimeDepSOUT << m_sigVTimeDepSOUT << m_sigdTwoTimeDepSOUT);
  }

  virtual void display(std::ostream &os) const {
    os << "Hello! My name is " << getName() << " !" << std::endl;
  }

  virtual const std::string &getClassName() const { return CLASS_NAME; }

  double &update(double &res, const int &inTime) {
    const double &aDouble = m_sigdSIN(inTime);
    res = aDouble;
    return res;
  }

  Vector &updateVector(Vector &res, const int &inTime) {
    const double &aDouble = m_sigdSIN(inTime);
    res.resize(2);
    res << aDouble, 2*aDouble;
    return res;
  }
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(MyEntity, "MyEntity");
} // namespace dynamicgraph

BOOST_AUTO_TEST_CASE(test_tracer) {
  using dynamicgraph::Vector;

  // Creates a tracer.
  dynamicgraph::Tracer &atracer = *dynamic_cast<dynamicgraph::Tracer *>(
      dynamicgraph::FactoryStorage::getInstance()->newEntity("Tracer",
                                                             "my-tracer"));

  dynamicgraph::Entity &entity =
      *dynamicgraph::FactoryStorage::getInstance()->newEntity("MyEntity",
                                                              "my-entity");

  std::string rootdir("/tmp");
  std::string basename("my-tracer");
  std::string suffix(".dat");

  /// Test openfiles
  atracer.openFiles(rootdir, basename, suffix);

  /// Add trace by name
  atracer.addSignalToTraceByName("my-entity.out_double", "output");
  /// Add trace by name
  atracer.addSignalToTraceByName("my-entity.out_vector", "output-vector");

  dynamicgraph::SignalBase<int> &aSignal = entity.getSignal("out2double");

  dynamicgraph::Signal<double, int> &aSignalInt =
      *(dynamic_cast<dynamicgraph::Signal<double, int> *>(
          &entity.getSignal("in_double")));

  dynamicgraph::Signal<Vector, int> &aSignalVector =
      *(dynamic_cast<dynamicgraph::Signal<Vector, int> *>(
          &entity.getSignal("out_vector")));

  /// Add trace by signal object
  atracer.addSignalToTrace(aSignal, "output2");

  aSignalInt.setConstant(1.5);

  atracer.start();

  for (int i = 0; i < 1000; i++) {
    aSignal.setTime(i);
    aSignalInt.access(i);
    aSignalInt.setTime(i);
    aSignalVector.recompute(i);
    aSignalVector.setTime(i);
    atracer.recordTrigger(i, i);
  }

  atracer.stop();
  atracer.clearSignalToTrace();
  atracer.closeFiles();

  atracer.record();
}
