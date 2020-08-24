/* Copyright 2019, LAAS-CNRS
 *
 * Olivier Stasse
 *
 */

#include <iostream>

#include <dynamic-graph/command.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/tracer-real-time.h>
#define BOOST_TEST_MODULE debug - tracer

#if BOOST_VERSION >= 105900
#include <boost/test/tools/output_test_stream.hpp>
#else
#include <boost/test/output_test_stream.hpp>
#endif
#include <boost/test/unit_test.hpp>
using boost::test_tools::output_test_stream;

namespace dynamicgraph {
struct MyEntity : public dynamicgraph::Entity {
  static const std::string CLASS_NAME;

  dynamicgraph::Signal<double, int> m_sigdSIN;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTimeDepSOUT;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTwoTimeDepSOUT;

  explicit MyEntity(const std::string &name)
      : Entity(name),
        m_sigdSIN("MyEntity(" + name + ")::input(double)::in_double"),
        m_sigdTimeDepSOUT(boost::bind(&MyEntity::update, this, _1, _2),
                          m_sigdSIN,
                          "MyEntity(" + name + ")::input(double)::out_double"),
        m_sigdTwoTimeDepSOUT(
            boost::bind(&MyEntity::update, this, _1, _2), m_sigdSIN,
            "MyEntity(" + name + ")::input(double)::out2double")

  {
    signalRegistration(m_sigdSIN << m_sigdTimeDepSOUT << m_sigdTwoTimeDepSOUT);
  }

  double &update(double &res, const int &inTime) {
    const double &aDouble = m_sigdSIN(inTime);
    res = aDouble;
    return res;
  }
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(MyEntity, "MyEntity");
} // namespace dynamicgraph

BOOST_AUTO_TEST_CASE(test_tracer) {
  using namespace dynamicgraph;

  // Creates a tracer.
  TracerRealTime &atracer = *dynamic_cast<TracerRealTime *>(
      FactoryStorage::getInstance()->newEntity("TracerRealTime", "my-tracer"));

  MyEntity &entity = *dynamic_cast<MyEntity *>(
      FactoryStorage::getInstance()->newEntity("MyEntity", "my-entity"));

  std::string rootdir("/tmp");
  std::string basename("my-tracer");
  std::string suffix(".dat");

  atracer.setBufferSize(1 << 14);

  // Check that an exception is thrown if the filename is invalid.
  atracer.openFiles(rootdir, "invalid/filename", suffix);
  BOOST_CHECK_THROW(
      atracer.addSignalToTraceByName("my-entity.out_double", "output"),
      ExceptionTraces);

  // Test openfiles
  atracer.openFiles(rootdir, basename, suffix);
  // Add trace by name
  atracer.addSignalToTraceByName("my-entity.out_double", "output");

  /// Add trace by name
  SignalBase<int> &out_double = entity.getSignal("out_double");
  SignalBase<int> &out_double_2 = entity.getSignal("out2double");

  Signal<double, int> &in_double =
      *(dynamic_cast<Signal<double, int> *>(&entity.getSignal("in_double")));

  in_double.setConstant(1.5);
  atracer.start();

  std::string emptybuf_cmd_str("empty");
  command::Command *acmd = atracer.getNewStyleCommand(emptybuf_cmd_str);
  acmd->execute();
  for (int i = 0; i < 1000; i++) {
    in_double.setTime(i);
    out_double.recompute(i);
    out_double_2.recompute(i);
    atracer.recordTrigger(i, i);
  }
  output_test_stream output;

  atracer.display(output);

  atracer.stop();
  atracer.trace();
  atracer.clearSignalToTrace();
  atracer.closeFiles();
  acmd->execute();
  atracer.record();

  BOOST_CHECK(output.is_equal(
      "TracerRealTime my-tracer [mode=play] : \n"
      "  - Dep list: \n"
      "     -> MyEntity(my-entity)::input(double)::out_double (in output)"
      "	[8Ko/16Ko]	\n"));
}
