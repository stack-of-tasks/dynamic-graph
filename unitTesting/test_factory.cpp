/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-JAPAN, Tsukuba, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      test_factory.cc
 * Project:   SOT
 * Author:    Nicolas Mansard
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

/* -------------------------------------------------------------------------- */
/* --- INCLUDES ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */


#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>

#include <dynamic-graph/factory.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/debug.h>


using namespace std;
using namespace dynamicgraph;

#ifdef WIN32
#include <Windows.h>
#else 
#include <dlfcn.h> 
#endif

#ifdef WIN32
	typedef HMODULE sotPluginKey;
#else
	typedef void* sotPluginKey;
#endif

int main()
{
  
  dgDEBUG(0) << "# In {"<<endl;
//   Entity test("");
//   sotExceptionFeature t2(sotExceptionFeature::BAD_INIT);
//   sotExceptionSignal t4(sotExceptionSignal::COPY_NOT_INITIALIZED);
//   sotFlags t3;
//   TestFeature t5;

#ifndef WIN32
  sotPluginKey dlib = dlopen("lib/plugin/sotFeatureVisualPoint.so", RTLD_NOW);
#else
  sotPluginKey dlib = LoadLibrary ( "lib/plugin/sotFeatureVisualPoint.lib"); 
#endif
if( NULL==dlib ) 
    {
      cerr << " Error dl"<<endl;
#ifndef WIN32
      cerr << dlerror() <<endl;
#else
    // Retrieve the system error message for the last-error code
    LPTSTR pszMessage;
    DWORD dwLastError = GetLastError(); 
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszMessage,
        0, NULL );


	  cerr << pszMessage <<endl;
	  LocalFree(pszMessage);
#endif

      exit(0);
    }


#ifndef WIN32
	dlib = dlopen("lib/plugin/sotGainAdaptative.so", RTLD_NOW);
#else
	dlib = LoadLibrary ("lib/plugin/sotGainAdaptative.lib"); 
#endif
  if( NULL==dlib ) 
    {
	  cout << "This test can only be run after the package StackOfTasks has been installed\n"
			  "Could not find sotGainAdaptive shared library" << endl;
#ifndef WIN32
      cerr << dlerror() <<endl;
#else
    // Retrieve the system error message for the last-error code
    LPTSTR pszMessage;
    DWORD dwLastError = GetLastError(); 
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszMessage,
        0, NULL );


	  cerr << pszMessage <<endl;
	  LocalFree(pszMessage);
#endif
      exit(0);
    }

  Entity* gain = 0;
  if ( !factory.existEntity("GainAdaptative") ) {
	  cout << "Could not find entity class 'GainAdaptative'" << endl;
	  exit(0);
	}
  else
	 gain = factory.newEntity("GainAdaptative","Gain");

  
  gain->display(cout); cout << endl;
  cout <<gain->getClassName(); cout << endl;


  dgDEBUG(0) << "# Out }"<<endl;
}

