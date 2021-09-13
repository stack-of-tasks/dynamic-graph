// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_FACTORY_HH
#define DYNAMIC_GRAPH_FACTORY_HH
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-factory.h>

#include <boost/noncopyable.hpp>
#include <dynamic-graph/fwd.hh>
#include <map>
#include <string>
#include <vector>

/// \ingroup dgraph
///
/// \brief Automatically register a class to the global factory
/// by relying on the static initialization.
///
/// \param CLASSTYPE the Entity type to be registered
/// \param CLASSNAME the name of the Entity to be registered (this must
///        be a std::string or a type implicitly castable into a std::string
///        such as classic C string delimited by double quotes).
#define DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(CLASSTYPE, CLASSNAME)              \
  const std::string CLASSTYPE::CLASS_NAME = CLASSNAME;                        \
  extern "C" {                                                                \
  ::dynamicgraph::Entity *EntityMaker_##CLASSTYPE(                            \
      const std::string &objname) {                                           \
    return new CLASSTYPE(objname);                                            \
  }                                                                           \
  ::dynamicgraph::EntityRegisterer reg_##CLASSTYPE(CLASSNAME,                 \
                                                   &EntityMaker_##CLASSTYPE); \
  }                                                                           \
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

namespace dynamicgraph {
/// \ingroup dgraph
///
/// \brief Provides a way to create Entity objects from their class
/// name.
///
/// The dynamic graph frameworks relies on entities (see Entity)
/// which defines atomic processing units. This class provides a
/// robust way to enumerate and instantiate these entities.
/// Each entity has a name (its type name) and can be instantiated.
/// Each instance also has a name.
///
/// For instance one can define a C++ class called MyEntity which
/// inherits from dynamicgraph::Entity. This type can be registered
/// into the factory to teach the framework that:
/// - this entity exists
/// - this entity can be instantiated (and how to instantiate it).
///
/// To achieve this, one must pass an entity name and a function pointer.
///
/// The entity name will identify the class <b>at run-time</b>
/// (be careful: this may not be equivalent to the C++ class name
/// even if it is recommended to do so).
///
/// The function pointer must point on a function taking a string as
/// input and returning an instance of the Entity (the concrete
/// subclass, not directly the upper Entity class).
///
/// The instances returned by this function <b>must</b> be
/// dynamically allocated and the caller <b>must</b> get the
/// ownership of the instance (i.e. it will free it when required).
///
/// To finish, please note that the instance name indicates to the
/// entity how the instance itself is called at run-time.  This name
/// does not need to be unique and no check is done on it.  It is
/// the caller responsibility to make sure that the instance name is
/// appropriate and to check for uniqueness if required.
///
///
/// This class is a singleton. The rationale is that each
/// unique name must identify a unique Entity. The use of a single
/// instance of this class enforces this behavior, instantiating one
/// yourself would break this property.
class DYNAMIC_GRAPH_DLLAPI FactoryStorage : private boost::noncopyable {
 public:
  /// \brief Function pointer providing an entity instance from its
  /// name.
  typedef Entity *(*EntityConstructor_ptr)(const std::string &);

  ~FactoryStorage();

  /// \brief Get pointer to unique object of the class
  static FactoryStorage *getInstance();

  /// \brief Destroy the unique instance of the class
  static void destroy();

  /// \brief Add a new entity to the factory.
  ///
  /// It is not allowed to have several entities using the same
  /// name. If this is the case, an ExceptionFactory exception will
  /// be raised with the code OBJECT_CONFLICT.
  ///
  /// If the function pointer is null, an ExceptionFactory exception
  /// will be raised with the code OBJECT_CONFLICT.
  ///
  /// \param entname the name used to subscribe the entity.
  /// \param ent pointer to a function allocating an entity from an
  /// instance name.
  void registerEntity(const std::string &entname, EntityConstructor_ptr ent);

  /// \brief Delete an entity from the factory.
  ///
  /// If the provided entity name does not exist in the factory,
  /// an ExceptionFactory exception will be raised with the code
  /// OBJECT_CONFLICT.
  ///
  /// \param entname the entity name (as passed to registerEntity before)
  void deregisterEntity(const std::string &entname);

  /// \brief Instantiate (and allocate) an entity.
  ///
  /// An instance called objname of the entity which type is classname
  /// will be allocated by this method.
  ///
  /// It is <b>the caller</b> responsibility to free the
  /// returned object.
  ///
  /// If the class name does not exist, an ExceptionFactory
  /// exception will be raised with the code UNREFERED_OBJECT.
  ///
  /// The instance name (objname) is passed to the Entity
  /// constructor and it is the caller responsibility to avoid
  /// instance name conflicts if necessary.
  ///
  /// \param classname the name of the Entity type
  /// \param objname the instance name
  /// \return Dynamically allocated instance of classname.
  Entity *newEntity(const std::string &classname,
                    const std::string &objname) const;

  /// \brief Check if an Entity associated with a particular name
  /// has already been registered.
  ///
  /// \param name entity name
  /// \return Do the entity exist?
  bool existEntity(const std::string &name) const;

  /// \brief List the available entities.
  ///
  /// Available entities are appended to the method argument.
  ///
  /// \param list Available entities will be appended to list.
  void listEntities(std::vector<std::string> &list) const;

 private:
  /// \brief Constructor the factory.
  ///
  /// After the initialization, no entities will be available.
  /// registerEntity has to be used to add new entities to the
  /// object.
  explicit FactoryStorage();

  /// \brief Entity map type.
  ///
  /// This maps entity names to functions pointers which can be
  /// used to instantiate an Entity.
  typedef std::map<std::string, EntityConstructor_ptr> EntityMap;

  /// \brief The entity map storing information about how to
  /// instantiate an Entity.
  EntityMap entityMap;

  /// \pointer to the unique object of the class
  static FactoryStorage *instance_;
};

/// \ingroup dgraph
///
/// \brief This class automatically register an Entity to the
/// global factory at initialization and unregister it during
/// instance destruction.
///
/// This class is mainly used by the
/// DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN macro and is of little interest
/// by itself.
class DYNAMIC_GRAPH_DLLAPI EntityRegisterer : private boost::noncopyable {
 public:
  /// \brief Register entity to the global factory.
  explicit EntityRegisterer(const std::string &entityClassName,
                            FactoryStorage::EntityConstructor_ptr maker);

  /// \brief Unregister entity to the global factory.
  ~EntityRegisterer();

 private:
  /// \brief Name of the entity registered when the instance has
  /// been initialized.
  const std::string entityName;
};
}  // end of namespace dynamicgraph

#endif  //! DYNAMIC_GRAPH_FACTORY_HH

//  LocalWords:  unregister
