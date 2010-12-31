// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// dynamic-graph. If not, see <http://www.gnu.org/licenses/>.

#include <cassert>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/exception-abstract.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/import-default-paths.h>
#include <dynamic-graph/import.h>
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/plugin-loader.h>

// The default import paths has to be passed from the build system
// as a -D flag for instance.
// It contains the absolute path to the default directory where
// the scripts will be searched.
#ifndef DG_IMPORT_DEFAULT_PATHS
# error "Default import path is not defined."
#endif //! DG_IMPORT_DEFAULT_PATHS

static const char* ENV_DG_PATH = "DG_PATH";


// Define OS specific extensions for shared libraries.
#ifdef _WIN32
static const char* SHARED_LIBRARY_EXT = ".dll";
#elif __APPLE__
static const char* SHARED_LIBRARY_EXT = ".dylib";
#else
static const char* SHARED_LIBRARY_EXT = ".so";
#endif

namespace dynamicgraph
{
  namespace command
  {
    namespace
    {
      /// Initialize import paths list (called during static initialization).
      paths_t initializePaths ();

      paths_t importPaths = initializePaths ();
      paths_t alreadyImportedPaths;

      /// Search for a module.
      ///
      /// Returns the module full absolute path or an empty string if
      /// it cannot be found.
      boost::filesystem::path searchModule (const std::string& module);

      /// \brief Remove quotes from a string.
      ///
      /// Transform strings such as "foo" or 'foo' into foo.
      void removeQuotes (std::string& msg);

      paths_t initializePaths ()
      {
	// Split the environment variable.
	std::string environmentSeparator;

	// On Microsoft Windows, environment variables are splitted along
	// the ``;'' character. It is ``:'' on *NIX systems.
#if defined _WIN32 || defined __CYGWIN__
	environmentSeparator = ";";
#else
	environmentSeparator = ":";
#endif // defined _WIN32 || defined __CYGWIN__


	// Declare variables that will be returned.
	paths_t importPaths;

	// Split the built-in values.
	std::vector<std::string> splittedDefaultPaths;
        boost::split (splittedDefaultPaths,
		      DG_IMPORT_DEFAULT_PATHS,
		      boost::is_any_of (environmentSeparator));

        // Insert them back.
	{
	  std::back_insert_iterator<paths_t> bi (importPaths);
	  std::copy (splittedDefaultPaths.begin (),
		     splittedDefaultPaths.end (), bi);
	}

	// Search for the environment variable value.
	char* ScriptPath = getenv (ENV_DG_PATH);
	if (!ScriptPath)
	  return importPaths;

	std::vector<std::string> splittedEnvironmentVariable;
        boost::split (splittedEnvironmentVariable, ScriptPath,
		      boost::is_any_of (environmentSeparator));

        // Insert it back
        std::back_insert_iterator<paths_t> bi (importPaths);
        std::copy (splittedEnvironmentVariable.begin (),
		   splittedEnvironmentVariable.end (), bi);
        return importPaths;
      }

      boost::filesystem::path searchModule (const std::string& module)
      {
	// To ensure correct priorities, the traversal has to
	// done left to right.
	typedef paths_t::const_iterator citer_t;
	for (citer_t it = importPaths.begin ();
	     it != importPaths.end (); ++it)
	  {
	    const boost::filesystem::path& dir = *it;
	    if (!boost::filesystem::is_directory (dir))
	      continue;

	    boost::filesystem::path path = dir / module;
	    boost::filesystem::path pathPlugin = path;
	    pathPlugin.replace_extension (SHARED_LIBRARY_EXT);

	    if (boost::filesystem::exists (pathPlugin)
		&& (boost::filesystem::is_regular_file (pathPlugin)
		    || boost::filesystem::is_symlink (pathPlugin)))
	      return pathPlugin;

	    if (boost::filesystem::exists (path)
		&& (boost::filesystem::is_regular_file (path)
		    || boost::filesystem::is_symlink (path)))
	      return path;
	  }

	return boost::filesystem::path ();
      }

      void removeQuotes (std::string& msg)
      {
	if ((msg[0] == '"' && msg[msg.length () - 1] == '"')
	    || (msg[0] == '\'' && msg[msg.length () - 1] == '\''))
	  msg = msg.substr (1, msg.length () - 2);
      }

      void importScript (dynamicgraph::Interpreter& interpreter,
			 const boost::filesystem::path& path,
			 std::ifstream& file,
			 std::ostream& os)
      {
	int lineIdx = 0;
	try
	  {
	    while (file.good ())
	      {
		dgDEBUGIN (15);
		++lineIdx;

		std::string line;
		std::getline (file, line);
		if (line.empty ())
		  continue;

		std::istringstream iss (line);
		std::string currentCmdName;
		std::string currentCmdArgs;
		if (iss >> currentCmdName)
		  {
		    std::getline (iss, currentCmdArgs);
		    boost::format fmt ("Run ``%1%'' with args ``%2%''");
		    fmt % currentCmdName % currentCmdArgs;
		    dgDEBUG(25) << fmt.str () << std::endl;
		    std::istringstream issArgs (currentCmdArgs);
		    interpreter.cmd (currentCmdName, issArgs, os);
		  }
		dgDEBUGOUT (15);
	      }
	  }
	catch (ExceptionAbstract& exc)
	  {
	    // FIXME: come on...
	    std::string& msg =
	      const_cast<std::string&> (exc.getStringMessage ());
	    boost::format fmt (" (in line %1% of file ``%2%'')");
	    fmt % lineIdx % path.file_string ();
	    msg = msg + fmt.str ();
	    std::cout << msg << std::endl;
	    throw;
	  }
      }

      void importPlugin (dynamicgraph::Interpreter& interpreter,
			 const boost::filesystem::path& path,
			 std::ostream&)
      {
	if (!interpreter.dlPtr)
	  {
	    DG_THROW ExceptionFactory
	      (ExceptionFactory::DYNAMIC_LOADING,
	       "No plug-in loader available.");
	    return;
	  }
	interpreter.dlPtr->addPlugin (path.filename (),
				      path.parent_path ().file_string ());
	interpreter.dlPtr->loadPlugins ();
      }
    } // end of anonymous namespace.

    void import (dynamicgraph::Interpreter& interpreter,
		 const std::string& cmdLine,
		 std::istringstream& cmdArg,
		 std::ostream& os)
    {
      if (cmdLine == "help")
	{
	  os << "  - import <script.txt>\t\t\t\tImport the script."
	     << std::endl
	     << "\t\t\t\tBehaves like run but searches for files"
	     << " in default script directories."
	     << std::endl;
	  return;
	}

      dgDEBUGIN(15);

      std::string module;
      cmdArg >> module;

      // Get rid of quotes.
      removeQuotes (module);

      // Get the absolute path of the module.
      boost::filesystem::path path = searchModule (module);

      // Check that the module can be opened.
      std::ifstream file (path.file_string ().c_str ());
      if (!boost::filesystem::is_regular_file (path)
	  || !file.is_open () || !file.good ())
	{
	  std::string scriptDirectories;

	  if (importPaths.empty ())
	    scriptDirectories = "empty";
	  else
	    {
	      BOOST_FOREACH (const boost::filesystem::path& path, importPaths)
		{
		  scriptDirectories += path.file_string ();
		  scriptDirectories += ", ";
		}
	      scriptDirectories = scriptDirectories.substr
		(0, scriptDirectories.length () - 2);
	    }

	  boost::format fmt
	    ("failed to import module ``%1%'' (import paths: %2%).");
	  fmt % module;
	  fmt % scriptDirectories;
	  DG_THROW ExceptionFactory
	    (ExceptionFactory::READ_FILE, fmt.str ());
	  return;
	}

      // If the module has already been imported, do not import it
      // again.
      if (std::find (alreadyImportedPaths.begin (),
		     alreadyImportedPaths.end (),
		     path) != alreadyImportedPaths.end ())
	return;

      if (path.extension () != SHARED_LIBRARY_EXT)
	importScript (interpreter, path, file, os);
      else
	importPlugin (interpreter, path, os);

      alreadyImportedPaths.push_back (path);

      dgDEBUGOUT(15);
    }

    void pushImportPaths (dynamicgraph::Interpreter&,
			  const std::string& cmdLine,
			  std::istringstream& cmdArg,
			  std::ostream& os)
    {
      if (cmdLine == "help")
	{
	  os << "  - pushImportPaths <path>\t\t\t\tAdd path to default directories."
	     << std::endl;
	  return;
	}
      std::string path;
      cmdArg >> path;
      removeQuotes (path);
      if (path.empty ())
	DG_THROW ExceptionFactory
	  (ExceptionFactory::SYNTAX_ERROR,
	   "pushImportPaths expects exactly one argument.");

      if (!cmdArg.eof ())
	DG_THROW ExceptionFactory
	  (ExceptionFactory::SYNTAX_ERROR,
	   "pushImportPaths expects exactly one argument.");

      importPaths.push_back (path);
    }

    void popImportPaths (dynamicgraph::Interpreter&,
			 const std::string& cmdLine,
			 std::istringstream&,
			 std::ostream& os)
    {
      if (cmdLine == "help")
	{
	  os << "  - popImportPaths <path>\t\t\t\tDrop path from default directories."
	     << std::endl;
	  return;
	}
      if (!importPaths.empty ())
	importPaths.pop_back ();
      else
	//FIXME: get a better error code for this.
	DG_THROW ExceptionFactory
	  (ExceptionFactory::SYNTAX_ERROR,
	   "Vector is already empty.");
    }

  } // end of namespace command.
} // end of namespace sot.
