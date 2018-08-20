// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software:
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <boost/foreach.hpp>

#include "dynamic-graph/debug.h"
#include "dynamic-graph/factory.h"

using namespace std;
using namespace dynamicgraph;

namespace dynamicgraph
{
  FactoryStorage* FactoryStorage::getInstance ()
  {
    if (instance_ == 0) {
      instance_ = new FactoryStorage;
    }
    return instance_;
  }

  void FactoryStorage::destroy()
  {
    delete instance_;
    instance_ = NULL;
  }

  FactoryStorage::FactoryStorage  ()
    : entityMap  ()
  {}

  FactoryStorage::~FactoryStorage  ()
  {
    instance_ = 0;
    dgDEBUGINOUT (25);
  }

  void
  FactoryStorage::registerEntity(const std::string& entname,
				 FactoryStorage::EntityConstructor_ptr ent)
  {
    dgDEBUGIN (25);
    if (existEntity (entname))
      {
	DG_THROW ExceptionFactory
	  (ExceptionFactory::OBJECT_CONFLICT,
	   "Another entity class already defined with the same name. ",
	   "(while adding entity class <%s> inside the factory).",
	   entname.c_str  ());
	dgERRORF ("Another entity class already defined with the same name. "
		  "(while adding entity class <%s> inside the factory).",
		  entname.c_str  ());
      }
    else
      {
	if (!ent)
	  {
	    //FIXME: we should have a better error code for that.
	    DG_THROW ExceptionFactory
	      (ExceptionFactory::OBJECT_CONFLICT,
	       "Bad entity constructor.");
	  }

	dgDEBUG (30) << "Register entity <"<< entname
		     << "> in the factory." <<std::endl;
	entityMap[entname] = ent;
      }
    dgDEBUGOUT (25);
  }

  void
  FactoryStorage::deregisterEntity (const std::string& entname)
  {
    dgDEBUGIN (25);
    if (!existEntity(entname))
      {
	DG_THROW ExceptionFactory( ExceptionFactory::OBJECT_CONFLICT,
				   "Entity class not defined yet. ",
				   "(while removing entity class <%s>).",
				   entname.c_str () );
	dgERRORF(ExceptionFactory::OBJECT_CONFLICT,
		 "Entity class not defined yet. "
		 "(while removing entity class <%s>).",
		 entname.c_str () );
      }
    else
      {
	dgDEBUG (30) << "Deregister entity <"<< entname
		     << "> from the factory." <<std::endl;
	entityMap.erase(entname);
      }
    dgDEBUGOUT (25);
  }

  Entity*
  FactoryStorage::newEntity (const std::string& classname,
			     const std::string& objname) const
  {
    dgDEBUG (15) << "New <" << classname << ">Entity <"
		 << objname << ">" << std::endl;

    EntityMap::const_iterator entPtr = entityMap.find (classname);
    if (entPtr == entityMap.end  ())
      {
	DG_THROW ExceptionFactory
	  (ExceptionFactory::UNREFERED_OBJECT,
	   "Unknown entity.",
	   " (while calling new_entity <%s>)",
	   classname.c_str  ());
      }
    return entPtr->second (objname);
  }


  // This checks efficiently if a key exists in an STL map using the
  // approach suggested by Scott Meyer's Effective STL (item 24).
  bool
  FactoryStorage::existEntity (const std::string& name) const
  {
    EntityMap::const_iterator lb = entityMap.lower_bound (name);
    return lb != entityMap.end  ()
      && !(entityMap.key_comp  () (name, lb->first));
  }

  //FIXME: this should be removed at some point.
  void
  FactoryStorage::listEntities (std::vector<std::string>& outList) const
  {
    typedef std::pair<std::string, EntityConstructor_ptr> iter_t;
    BOOST_FOREACH (const iter_t& entity, entityMap)
      outList.push_back(entity.first);
  }

  EntityRegisterer::EntityRegisterer
  (const std::string& entityClassName, FactoryStorage::EntityConstructor_ptr maker)
    : entityName (entityClassName)
  {
    dgDEBUGIN (15);
    FactoryStorage::getInstance()->registerEntity (entityClassName, maker);
    dgDEBUGOUT (15);
  }

  EntityRegisterer::~EntityRegisterer ()
  {
    dgDEBUGIN(15);
    FactoryStorage::getInstance()->deregisterEntity (entityName);
    dgDEBUGOUT (15);
  }


  // The global factory.
  FactoryStorage* FactoryStorage::instance_ = NULL;
} // end of namespace dynamicgraph.
