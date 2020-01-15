/* Copyright 2019, LAAS-CNRS
 *
 * Olivier Stasse
 *
 * See LICENSE file
 *
 */
#include "dynamic-graph/command-bind.h"
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

using namespace dynamicgraph::command;

namespace dynamicgraph {
class CustomEntity : public Entity {
public:
  static const std::string CLASS_NAME;
  bool test_zero_arg_;
  bool test_one_arg_;
  bool test_two_args_;
  bool test_three_args_;
  bool test_four_args_;

  virtual const std::string &getClassName() const { return CLASS_NAME; }
  CustomEntity(const std::string n) : Entity(n) {
    test_zero_arg_ = false;
    test_one_arg_ = false;
    test_two_args_ = false;
    test_three_args_ = false;
    test_four_args_ = false;

    addCommand("0_arg", makeCommandVoid0(*this, &CustomEntity::zero_arg,
                                         docCommandVoid0("zero arg")));

    addCommand("1_arg", makeCommandVoid1(*this, &CustomEntity::one_arg,
                                         docCommandVoid1("one arg", "int")));

    addCommand("2_args",
               makeCommandVoid2(*this, &CustomEntity::two_args,
                                docCommandVoid2("two args", "int", "int")));

    addCommand("3_args", makeCommandVoid3(*this, &CustomEntity::three_args,
                                          docCommandVoid3("three args", "int",
                                                          "int", "int")));

    addCommand("4_args",
               makeCommandVoid4(
                   *this, &CustomEntity::four_args,
                   docCommandVoid4("four args", "int", "int", "int", "int")));
  }

  ~CustomEntity() {}

  void zero_arg() { test_zero_arg_ = true; }

  void one_arg(const int &) { test_one_arg_ = true; }

  void two_args(const int &, const int &) { test_two_args_ = true; }

  void three_args(const int &, const int &, const int &) {
    test_three_args_ = true;
  }

  void four_args(const int &, const int &, const int &, const int &) {
    test_four_args_ = true;
  }
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(CustomEntity, "CustomEntity");
} // namespace dynamicgraph

BOOST_AUTO_TEST_CASE(command_test) {
  dynamicgraph::CustomEntity &entity =
      *(dynamic_cast<dynamicgraph::CustomEntity *>(
          dynamicgraph::FactoryStorage::getInstance()->newEntity("CustomEntity",
                                                                 "my-entity")));

  std::map<const std::string, Command *> aCommandMap =
      entity.getNewStyleCommandMap();

  std::map<const std::string, Command *>::iterator it_map;

  it_map = aCommandMap.find("0_arg");
  if (it_map == aCommandMap.end())
    BOOST_CHECK(false);
  it_map->second->execute();
  BOOST_CHECK(entity.test_zero_arg_);

  int first_arg = 1;
  Value aValue(first_arg);
  std::vector<std::string> vec_fname(4);
  vec_fname[0] = "1_arg";
  vec_fname[1] = "2_args";
  vec_fname[2] = "3_args";
  vec_fname[3] = "4_args";
  std::vector<Value> values;

  for (unsigned int i = 0; i < 4; i++) {
    it_map = aCommandMap.find(vec_fname[i]);
    if (it_map == aCommandMap.end())
      BOOST_CHECK(false);
    values.push_back(aValue);
    it_map->second->setParameterValues(values);
    it_map->second->execute();
    it_map->second->owner();
    it_map->second->getDocstring();
  }
  
  BOOST_CHECK(entity.test_one_arg_);
  BOOST_CHECK(entity.test_two_args_);
  BOOST_CHECK(entity.test_three_args_);
  BOOST_CHECK(entity.test_four_args_);

  std::vector<Value> values_two;
  values_two.push_back(aValue);
  /// Wrong number of arguments
  bool res=false;
  it_map = aCommandMap.find(std::string("2_args"));
  try {
    it_map->second->setParameterValues(values_two);
  }
  catch (const dynamicgraph::ExceptionAbstract &aea)
  {
    res = (aea.getCode() == dynamicgraph::ExceptionAbstract::ABSTRACT);
  }
  BOOST_CHECK(res);

  double snd_arg_db=10.0;
  Value aValue2(snd_arg_db);
  values_two.push_back(aValue2);

  /// Wrong types of arguments
  res=false;
  it_map = aCommandMap.find(std::string("2_args"));
  try {
    it_map->second->setParameterValues(values_two);
  }
  catch (const dynamicgraph::ExceptionAbstract &aea)
  {
    res = (aea.getCode() == dynamicgraph::ExceptionAbstract::TOOLS);
  }
  BOOST_CHECK(res);
  
  

}
