/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of dynamic-graph.
 * dynamic-graph is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * dynamic-graph is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.
 */

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

/* --- DYNAMIC-GRAPH --- */
#include <dynamic-graph/plugin-loader.h>
#include <dynamic-graph/debug.h>

/* --- STD --- */
#include <fstream>
#include <sstream>
using namespace std;
using namespace dynamicgraph;

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */
class dynamicgraph::PluginRefMap
{
public:

#ifdef WIN32
	typedef HMODULE plugin_key_type;
#else
	typedef void* plugin_key_type;
#endif

	typedef std::map< std::string,plugin_key_type> KeyMap;
	KeyMap keyMap;
};


PluginLoader::
PluginLoader( void )
{
  pluginRefs = new PluginRefMap();
}

PluginLoader::
~PluginLoader( void )
{
  delete pluginRefs;
}

const std::string& PluginLoader::
setDirectory( const std::string& n )
{
  return pluginDirectory = n;
}
const std::string& PluginLoader::
getDirectory( void )
{
  return pluginDirectory;
}

void PluginLoader::
loadPluginList( const std::string& configFileName, const std::string& dir )
{
  dgDEBUGIN(15);


  string name;
  //while (1)
  ifstream configFile( configFileName.c_str(),ios::in );
  if(! configFile.is_open())
    {
      DG_THROW ExceptionFactory( ExceptionFactory::READ_FILE,
				     "File is not open."," (while reading <%s>).",
				     configFileName.c_str() );
    }

  for(;;)
    {
      configFile>>name;
      if(configFile.eof()) break;
      dgDEBUG(9)<<"Add <"<< name << "> to the list"<<endl;
      addPlugin( name,dir);
    }

  dgDEBUGOUT(15);
}

void PluginLoader::
addPlugin( const std::string& name, const std::string& dir )
{
  dgDEBUGIN(15);
  if( dir.length() )
    pluginNames.push_back(  dir +"/"+name );
  else
    {      pluginNames.push_back(  pluginDirectory +"/"+name );    }
  dgDEBUGOUT(15);
}





/* -------------------------------------------------------------------------- */
/* --- DYNAMIC LOADER LIB --------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void PluginLoader::
loadPlugins( void )
{
  dgDEBUGIN(15);

  for( list<string>::iterator iter = pluginNames.begin();
       iter!=pluginNames.end();++iter )
    {
      dgDEBUG(9)<<"Load <"<< *iter << "> plugin"<<endl;
#ifndef WIN32
      PluginRefMap::plugin_key_type dlib = dlopen( iter->c_str(),RTLD_NOW|RTLD_GLOBAL);
#else
      PluginRefMap::plugin_key_type dlib = LoadLibrary ( iter->c_str());
#endif

      dgDEBUG(19)<<"Plugin <"<< *iter << "> loaded "<<endl;

      if( NULL==dlib )
	{
	  std::string wrongLib = *iter;
	  pluginNames.erase(iter);

	  // FIXME: this line has been added to avoid, since erasing
	  // an element from a list invalidates the underlying
	  // iterator.  However I am not sure whether loading twice
	  // the plug-ins is safe or not and should be thoroughly
	  // tested.
	  //
	  // Under Linux this line should have no effect as an
	  // exception is thrown just after. MS Windows do not raise
	  // an exception here and must have an incoherent behavior
	  // anyway...
	  iter = pluginNames.begin();
#ifndef WIN32
	  dgDEBUG(5) << "Failure while loading: " <<dlerror() <<endl;
	  DG_THROW ExceptionFactory( ExceptionFactory::DYNAMIC_LOADING,
	  				 "Error while dlopen. ","<%s>",dlerror() );
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


	  dgDEBUG(5) << "Failure while loading: " << pszMessage <<endl;

	  std::ostringstream error_of;
	  error_of << "Error while LoadLibrary (" << wrongLib << ") ";

	  DG_THROW ExceptionFactory( ExceptionFactory::DYNAMIC_LOADING,
		  error_of.str().c_str(), pszMessage );

	  LocalFree(pszMessage);

#endif
	}

      loadedPluginNames[*iter] = (*iter);
	  pluginRefs->keyMap[*iter] = dlib;
    }
  pluginNames.clear();

  dgDEBUGOUT(15);
}

/* -------------------------------------------------------------------------- */
/* --- DYNAMIC UN-LOADER LIB ------------------------------------------------ */
/* -------------------------------------------------------------------------- */


void PluginLoader::
unloadPlugin( const std::string& plugname )
{
  dgDEBUGIN( 15 );

  PluginRefMap::KeyMap::iterator plugkey = pluginRefs->keyMap.find(plugname);
  if( plugkey==pluginRefs->keyMap.end() ) // key does exist
    {
      throw ExceptionFactory( ExceptionFactory::OBJECT_CONFLICT,
				 "Plugin not loaded",": <%s>.",plugname.c_str() );
    }

#ifndef WIN32
  const int errCode = dlclose(plugkey->second);
#else
  const int errCode = FreeLibrary(plugkey->second);
#endif
  if( errCode )
    {
#ifndef WIN32
      dgDEBUG(1) << "Error while unloading <" << plugname <<"> : "
		  << dlerror() <<endl;
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

		dgDEBUG(1) << "Error while unloading <" << plugname <<"> : "
		  << pszMessage <<endl;

	  LocalFree(pszMessage);
#endif
    }

  //pluginRefs.erase( plugkey );

  dgDEBUGOUT( 15 );
}

const std::string& PluginLoader::
searchPlugin( const std::string& plugname )
{
  unsigned int refFound = 0;
  const std::string *plugFullName =0;
  for( PluginRefMap::KeyMap::iterator iter = pluginRefs->keyMap.begin();
       iter!=pluginRefs->keyMap.end();++iter )
    {
      const std::string &str = iter->first;
      size_t found = str.find_last_of("/\\");
      dgDEBUG(15) << " folder: " << str.substr(0,found) << std::endl;
      dgDEBUG(15) << " file: " << str.substr(found+1) << std::endl;
      if( str.substr(found+1)==plugname )
	{
	  refFound++;
	  plugFullName=&iter->first;
	}
    }

  if( 1!=refFound ) throw 1;

  return *plugFullName;
}


void PluginLoader::
unloadAllPlugins()
{
  dgDEBUGIN( 15 );

  PluginRefMap::KeyMap::iterator plugkey = pluginRefs->keyMap.begin();

  while( plugkey!=pluginRefs->keyMap.end() )
    {
#ifndef WIN32
      const int errCode = dlclose(plugkey->second);
#else
      const int errCode = FreeLibrary(plugkey->second);
#endif
      if( errCode )
	{
#ifndef WIN32
	  dgDEBUG(1) << "Error while unloading <" << plugkey->first <<"> : "
		      << dlerror() <<endl;
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

	  dgDEBUG(1) << "Error while unloading <" << plugkey->first <<"> : "
		      << pszMessage <<endl;

	  LocalFree(pszMessage);
#endif
	}
      plugkey++;
    }

  //pluginRefs.erase( plugkey );

  dgDEBUGOUT( 15 );
}
