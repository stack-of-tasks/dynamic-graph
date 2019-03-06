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

#define BOOST_TEST_MODULE debug-tracer

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

namespace dynamicgraph
{
  struct MyEntity : public dynamicgraph::Entity
  {
    static const std::string CLASS_NAME;

    dynamicgraph::Signal<double, int> m_sigdSIN;
    dynamicgraph::SignalTimeDependent<double, int> m_sigdTimeDepSOUT;
    dynamicgraph::SignalTimeDependent<double, int> m_sigdTwoTimeDepSOUT;

    MyEntity (const std::string& name)
      : Entity (name)
      ,m_sigdSIN("MyEntity("+name+")::input(double)::in_double")
      ,m_sigdTimeDepSOUT(boost::bind(&MyEntity::update,this,_1,_2),
			 m_sigdSIN,
			 "MyEntity("+name+")::input(double)::out_double")
      ,m_sigdTwoTimeDepSOUT(boost::bind(&MyEntity::update,this,_1,_2),
			    m_sigdSIN,
			    "MyEntity("+name+")::input(double)::out2double")

    {
      signalRegistration(m_sigdSIN
			 << m_sigdTimeDepSOUT
			 << m_sigdTwoTimeDepSOUT);
    }

    virtual void display (std::ostream& os) const
    {
      os << "Hello! My name is " << getName () << " !" << std::endl;
    }

    virtual const std::string& getClassName () const
    {
      return CLASS_NAME;
    }

    double & update(double &res, const int &inTime)
    {
      const double &aDouble = m_sigdSIN(inTime);
      res = aDouble;
      return res;
    }

  };
  DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN (MyEntity,"MyEntity");
}

BOOST_AUTO_TEST_CASE(test_tracer)
{

  // Creates a tracer.
  dynamicgraph::Tracer & atracer =
    *dynamic_cast<dynamicgraph::Tracer *>
    (dynamicgraph::FactoryStorage::getInstance()->newEntity("Tracer",
							    "my-tracer"));

  dynamicgraph::Entity & entity =
    *dynamicgraph::FactoryStorage::getInstance()->newEntity("MyEntity",
							    "my-entity");

  /// Add trace by name
  atracer.addSignalToTraceByName("my-entity.out_double","output");


  dynamicgraph::SignalBase<int> & aSignal =
    entity.getSignal("out2double");

  dynamicgraph::Signal<double,int> & aSignalInt =
    *(dynamic_cast<dynamicgraph::Signal<double,int> *>
      (& entity.getSignal("in_double")));

  /// Add trace by signal object
  atracer.addSignalToTrace(aSignal,"output2");

  aSignalInt.setConstant(1.5);

  std::string rootdir("/tmp");
  std::string basename("my-tracer");
  std::string suffix(".dat");

  /// Test openfiles
  atracer.openFiles(rootdir,basename,suffix);

  atracer.start();

  for(int i=0;i<1000;i++)
    {
      aSignal.setTime(i);
      aSignalInt.setTime(i);
      atracer.recordTrigger(i,i);
    }

  atracer.stop();

  atracer.closeFiles();
}
