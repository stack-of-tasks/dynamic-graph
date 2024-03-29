// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_ENTITY_H
#define DYNAMIC_GRAPH_ENTITY_H
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/logger.h>
#include <dynamic-graph/signal-array.h>
#include <dynamic-graph/signal-base.h>

#include <boost/noncopyable.hpp>
#include <dynamic-graph/fwd.hh>
#include <iosfwd>
#include <map>
#include <sstream>
#include <string>

/// \brief Helper macro for entity declaration.
///
/// This macro should be called in the declaration of all entities.
/// Example:
/// <code>
/// class A : public dynamicgraph::entity
/// {
///   DYNAMIC_GRAPH_ENTITY_DECL();
///
///   public:
//      // your class here
/// };
/// </code>
///
/// Caution: you *MUST* call DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN in the
/// associated source file to ensure that the attributes generated by
/// this macro are correctly initialized.
#define DYNAMIC_GRAPH_ENTITY_DECL()                                      \
 public:                                                                 \
  virtual const std::string &getClassName() const { return CLASS_NAME; } \
  static const std::string CLASS_NAME

namespace dynamicgraph {
/// \ingroup dgraph
///
/// \brief This class represents an entity, i.e. a generic
/// computational unit that provides input and output signals.
///
/// These signals link the entities together to form a complete
/// computation graph.  To declare a new entity, please see the
/// DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN macro in factory.h.
class DYNAMIC_GRAPH_DLLAPI Entity : private boost::noncopyable {
 public:
  typedef std::map<std::string, SignalBase<int> *> SignalMap;
  typedef std::map<const std::string, command::Command *> CommandMap_t;

  explicit Entity(const std::string &name);
  virtual ~Entity();

  const std::string &getName() const { return name; }
  virtual const std::string &getClassName() const {
    static std::string ret("Entity");
    return ret;
  }
  /** \brief Returns the Entity documentation
      \return The documentation is provided as std::string object.
   */
  virtual std::string getDocString() const;

  /** \brief Test if a signal of name signame is present.
      \return True if the signal is present, False otherwise
   */
  bool hasSignal(const std::string &signame) const;

  /** \brief Provides a reference to the signal named signalName.
      \param signalName: Name of the signal
      \return A reference to the signal with a temporal dependency.
   */
  SignalBase<int> &getSignal(const std::string &signalName);

  /** \brief Provides a const reference to the signal named signalName.
      \param signalName: Name of the signal
      \return A const reference to the signal with a temporal dependency.
   */
  const SignalBase<int> &getSignal(const std::string &signalName) const;

  /** \brief Display the list of signals of this entity in output stream os.
      \param os: the output stream where to display the list of signals.
      \returns The output stream given in parameter.
  */
  std::ostream &displaySignalList(std::ostream &os) const;

  /** \brief This method is used to write down in os the edges of the graph
      by calling the signals writeGraph method.
      \param os: The output stream where to write the informations.
      \return os: The output stream.
   */
  virtual std::ostream &writeGraph(std::ostream &os) const;

  /** \brief This method is used write in the output stream os the
      signals names and the commands of the entity.
      \param os: The output stream where to write the list of objects
      related to the entity.
   */
  virtual std::ostream &writeCompletionList(std::ostream &os) const;

  /** \brief Display information on the entity inside the output stream os.
   */
  virtual void display(std::ostream &os) const;

  virtual SignalBase<int> *test() { return 0; }

  virtual void test2(SignalBase<int> *) { return; }

  const std::string &getCommandList() const;

  /** \brief Provides the std::map where all the commands are registered
      \returns A map of pointers towards Command objects
  */
  CommandMap_t getNewStyleCommandMap();
  /** \brief Provides the pointer towards the Command object cmdName.
      \param cmdName: Name of the command
  */
  command::Command *getNewStyleCommand(const std::string &cmdName);

  /** \brief Provides a map of all the signals.
      \returns A copy of the map with all the pointers towards
      the entity signals.
   */
  SignalMap getSignalMap() const;

  /// \name Logger related methods
  /// \{

  Logger &logger() { return logger_; };
  Logger const &logger() const { return logger_; };

  /// \brief Send messages \c msg with level \c t.
  /// Add string file and line to message.
  void sendMsg(const std::string &msg, MsgType t = MSG_TYPE_INFO,
               const std::string &lineId = "");

  /// \brief Specify the verbosity level of the logger.
  void setLoggerVerbosityLevel(LoggerVerbosity lv) { logger_.setVerbosity(lv); }

  /// \brief Get the logger's verbosity level.
  LoggerVerbosity getLoggerVerbosityLevel() { return logger_.getVerbosity(); }

  /// \brief Set the time sample.
  bool setTimeSample(double t) { return logger_.setTimeSample(t); }

  /// \brief Get the time sample.
  double getTimeSample() { return logger_.getTimeSample(); }

  /// \brief Set the period of the stream period
  bool setStreamPrintPeriod(double t) {
    return logger_.setStreamPrintPeriod(t);
  }

  /// \brief Get the period of the stream period
  double getStreamPrintPeriod() { return logger_.getStreamPrintPeriod(); }

  /// \}

 protected:
  void addCommand(const std::string &name, command::Command *command);

  void entityRegistration();
  void entityDeregistration();

  void signalRegistration(const SignalArray<int> &signals);
  void signalDeregistration(const std::string &name);

  std::string name;
  SignalMap signalMap;
  CommandMap_t commandMap;
  Logger logger_;
};

DYNAMIC_GRAPH_DLLAPI std::ostream &operator<<(std::ostream &os,
                                              const dynamicgraph::Entity &ent);
}  // end of namespace dynamicgraph

#endif  //! DYNAMIC_GRAPH_ENTITY_H
