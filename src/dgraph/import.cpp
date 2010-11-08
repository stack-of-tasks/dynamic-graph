/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of sot-core.
 * sot-core is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * sot-core is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with sot-core.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <dynamic-graph/import-default-paths.h>
#include <dynamic-graph/import.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/exception-abstract.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/interpreter.h>

// The default import paths has to be passed from the build system
// as a -D flag for instance.
// It contains the absolute path to the default directory where
// the scripts will be searched.
#ifndef DG_IMPORT_DEFAULT_PATHS
# error "Default import path is not defined."
#endif //! DG_IMPORT_DEFAULT_PATHS

static const char* ENV_DG_PATH = "DG_PATH";

namespace dynamicgraph
{
  namespace command
  {
    namespace
    {
      /// Initialize import paths list (called during static initialization).
      std::vector<std::string> initializePaths ();

      /// \brief Import paths list.
      ///
      /// This vector of string is similar to Unix variables such as
      /// PATH. It contains all paths that are used to search when
      /// importing a script.
      ///
      /// The look-up is made from right to left:
      ///
      /// importPaths = A:B:C
      ///
      /// When typing ``import foo'', C will be searched first then B
      /// and A. The search stops when the file is found.
      std::vector<std::string> importPaths = initializePaths ();

      /// Search for a module.
      ///
      /// Returns the module full absolute path or an empty string if
      /// it cannot be found.
      std::string searchModule (const std::string& module);

      /// \brief Remove quotes form a string.
      ///
      /// Transform strings such as "foo" or 'foo' into foo.
      void removeQuotes (std::string& msg);

      std::vector<std::string> initializePaths ()
      {
	std::vector<std::string> importPaths;
	importPaths.push_back (DG_IMPORT_DEFAULT_PATHS);

	// Search for the environment variable value.
	char* ScriptPath = getenv (ENV_DG_PATH);
	if (!ScriptPath)
	  return importPaths;

	// Split the environment variable.
	std::string environmentSeparator;

	// On Microsoft Windows, environment variables are splitted along
	// the ``;'' character. It is ``:'' on *NIX systems.
#if defined _WIN32 || defined __CYGWIN__
	environmentSeparator = ";";
#else
	environmentSeparator = ":";
#endif // defined _WIN32 || defined __CYGWIN__

	std::vector<std::string> splittedEnvironmentVariable;
        boost::split (splittedEnvironmentVariable, ScriptPath,
		      boost::is_any_of (environmentSeparator));

        // Insert it back.
        std::back_insert_iterator<std::vector<std::string> > bi (importPaths);
        std::copy (splittedEnvironmentVariable.begin (),
		   splittedEnvironmentVariable.end (), bi);
        return importPaths;
      }

      std::string searchModule (const std::string& module)
      {
	// Make sure the traversal is right to left to enforce
	// correct priorities.
	typedef std::vector<std::string>::const_reverse_iterator citer_t;
	for (citer_t it = importPaths.rbegin ();
	     it != importPaths.rend (); ++it)
	  {
	    const std::string& path = *it;

	    assert (!path.empty ());

	    std::string filename (path);
	    if (filename[filename.length () - 1] != '/')
	      filename += "/";
	    filename += module;
	    std::ifstream file (filename.c_str ());
	    if (file.is_open () && file.good ())
	      return filename;
	  }
	return std::string ();
      }

      void removeQuotes (std::string& msg)
      {
	if ((msg[0] == '"' && msg[msg.length () - 1] == '"')
	    || (msg[0] == '\'' && msg[msg.length () - 1] == '\''))
	  msg = msg.substr (1, msg.length () - 2);
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

      std::string filename = searchModule (module);
      std::ifstream file (filename.c_str ());
      if (filename.empty () || !file.is_open () || !file.good ())
	{
	  std::string scriptDirectories;

	  if (importPaths.empty ())
	    scriptDirectories = "empty";
	  else
	    {
	      BOOST_FOREACH (const std::string& path, importPaths)
		{
		  scriptDirectories += path;
		  scriptDirectories += ", ";
		}
	      scriptDirectories = scriptDirectories.substr
		(0, scriptDirectories.length () - 2);
	    }

	  boost::format fmt
	    ("failed to import module ``%1%'' (import paths: %2%).");
	  fmt % module;
	  fmt % scriptDirectories;
	  std::cout << fmt.str () << std::endl;
	  DG_THROW ExceptionFactory
	    (ExceptionFactory::READ_FILE, fmt.str ());
	  return;
	}

      int lineIdx = 0;
      try
	{
	  while (file.good ())
	    {
	      ++lineIdx;
	      dgDEBUGIN (15);

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
	  std::string& msg = const_cast<std::string&> (exc.getStringMessage ());
	  boost::format fmt (" (in line %1% of file ``%2%'')");
	  fmt % lineIdx % filename;
	  msg = msg + fmt.str();
	  std::cout << msg << std::endl;
	  throw;
	}

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
    }

  } // end of namespace command.
} // end of namespace sot.
