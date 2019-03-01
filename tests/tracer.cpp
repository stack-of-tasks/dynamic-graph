/* Copyright 2019, LAAS-CNRS
 *
 * Olivier Stasse
 *
 * See LICENSE file
 *
 */

#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>

#include <dynamic-graph/tracer.h>

struct MyEntity : public dynamicgraph::Entity
{
  static const std::string CLASS_NAME;

  dynamicgraph::SignalPtr<double, int> m_sigdSIN;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTimeDepSOUT;
  
  MyEntity (const std::string& name)
    : Entity (name)
    ,m_sigdSIN(NULL,"MyEntity("+name+")::input(double)::in_double")
    ,m_sigdTimeDepSOUT(boost::bind(&MyEntity::update,this,_1,_2),
		       m_sigdSIN,
		       "MyEntity("+name+")::input(double)::out_double")
  {
    signalRegistration(m_sigdSIN << m_sigdTimeDepSOUT);
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

BOOST_AUTO_TEST_CASE(test_tracer)
{

  // Creates a tracer.
  dynamicgraph::Entity& entity =
    *dynamicgraph::FactoryStorage::getInstance()->newEntity("Tracer",
							    "my-tracer");

  
  
}
