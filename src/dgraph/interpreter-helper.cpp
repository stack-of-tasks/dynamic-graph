/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      interpretor.cpp
 * Project:   DYNAMIC-GRAPH
 * Author:    François Bleibel, Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* DYNAMIC-GRAPH */
#include <dynamic-graph/interpreter-helper.h>
#include <dynamic-graph/plugin-loader.h>
#include <dynamic-graph/debug.h>

/* --- STD --- */
using namespace std;
using namespace dynamicgraph;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */


InterpreterHelper::InterpreterHelper()
{
}


/* --------------------------------------------------------------------- */
/* --- NEW ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
#include <dynamic-graph/factory.h>
using namespace std;
#include <dynamic-graph/entity.h>
#include <dynamic-graph/signal-base.h>

void InterpreterHelper::
cmdPlug( const std::string& obj1, const std::string & signame1,
	 const std::string& obj2, const std::string & signame2,
	 std::ostream& os )
{
  dgDEBUG(20) << "Get Ent1 <"<<obj1<<"> ."<<endl;
  Entity& ent1 = g_pool.getEntity(obj1);
  dgDEBUG(20) << "Get Sig1 <"<<signame1<<"> ."<<endl;
  SignalBase<int> &sig1 = ent1.getSignal(signame1);

  dgDEBUG(20) << "Get Ent2 <"<<obj2<<"> ."<<endl;
  Entity& ent2 = g_pool.getEntity(obj2);
  dgDEBUG(20) << "Get Sig2 <"<<signame2<<"> ."<<endl;
  SignalBase<int> &sig2 = ent2.getSignal(signame2);

  dgDEBUG(25) << "Plug..."<<endl;
  sig2.plug(&sig1);
}

void InterpreterHelper::
cmdNew( const std::string& className,
	const std::string& objName,
	std::ostream& os )
{
  dgDEBUG(15) << "New <" << className<<"> requested."<<endl;
  if( g_factory.existEntity( className ) )
    {
      dgDEBUG(15) << "New entity<"<<className<<"> " <<objName<<std::endl;
      g_factory.newEntity(className,objName);
    }
  else os << "  !! Class <" << className << "> does not exist."<<endl;
}


void InterpreterHelper::
cmdDestroy( const std::string& objName,
	    std::ostream& os)
{
 
  dgDEBUG(15) << "Destroy <" << objName <<"> requested."<<endl;
  delete &( g_pool.getEntity( objName ) );

}

void InterpreterHelper::
cmdLoadPlugin( const std::string& directory, 
	       const std::string& pluginName,
	       std::ostream& os )
{

  if( directory.length() != 0 ) dlPtr.setDirectory( directory );
  dlPtr.addPlugin( pluginName );
  
  try{
    dgDEBUG(15) << "Try to load  " << pluginName<< endl;
    dlPtr.loadPlugins();
  }catch( ExceptionAbstract& e ) { dgDEBUG(5) << e << endl; throw e; }
}

void InterpreterHelper::
cmdUnloadPlugin( const std::string& pluginName,
		 std::ostream& os )
{

  dgDEBUGIN(15);
  try{
    dgDEBUG(25) << "Try short name " << pluginName << endl;
    const std::string& fullname = dlPtr.searchPlugin(pluginName);
    dgDEBUG(25) << "Full name " << fullname << endl;
    dlPtr.unloadPlugin(fullname);
  }
  catch(...)
    {
      dgDEBUG(25) << "Full name " << pluginName << endl;
      dlPtr.unloadPlugin(pluginName);
    }

  dgDEBUGOUT(15);
}

void InterpreterHelper::
cmdSetSignal( const std::string& objname, 
	      const std::string& signame,
	      const std::string& value,
	      std::ostream& os )
{
  dgDEBUGIN(15);

  Entity& obj = g_pool.getEntity(objname);
  SignalBase<int>& sig = obj.getSignal( signame );

  istringstream cmdArg(value);
  sig.set( cmdArg );

  dgDEBUGOUT(15);

}

void InterpreterHelper::
cmdGetSignal( const std::string& objname, 
	      const std::string& signame, 
	      std::ostream& os )
{
  dgDEBUGIN(15);

  Entity& obj = g_pool.getEntity(objname);
  SignalBase<int>& sig = obj.getSignal( signame );

  os << signame << " = "; sig.get( os );

  dgDEBUGOUT(15);

}

void InterpreterHelper::
cmdComputeSignal( const std::string& objname, 
		  const std::string& signame, 
		  const int &time,
		  std::ostream& os )
{
  dgDEBUGIN(15);

  Entity& obj = g_pool.getEntity(objname);
  SignalBase<int>& sig = obj.getSignal( signame );

  sig.recompute( time );

  dgDEBUGOUT(15);

}


/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

