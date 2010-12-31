// -*- c++-mode -*-
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

#ifndef SOT_FACTORY_COMMAND_IMPORT_H
# define SOT_FACTORY_COMMAND_IMPORT_H
# include <iosfwd>
# include <string>
# include <vector>

# include <boost/filesystem/path.hpp>

namespace dynamicgraph
{
  class Interpreter;
  namespace command
  {
    namespace
    {
      /// \brief Vector of Boost.Filesystem paths.
      ///
      /// Used to store paths where the import statement
      /// will look for scripts or plug-ins.
      typedef std::vector<boost::filesystem::path> paths_t;

      extern paths_t importPaths;
    } // end of anonymous namespace.

    /// \brief Implement sot interpretor import command.
    ///
    /// The import command sources a file and searches automatically
    /// for it in the importPaths.
    void import (Interpreter& interpretor,
		 const std::string& cmdLine,
		 std::istringstream& cmdArg,
		 std::ostream& os);

    /// \brief Implement sot interpretor pushImportPaths command.
    ///
    /// Append a path to importPaths.
    void pushImportPaths (Interpreter& interpretor,
			  const std::string& cmdLine,
			  std::istringstream& cmdArg,
			  std::ostream& os);

    /// \brief Implement sot interpretor popImportPaths command.
    ///
    /// Drop the last path of importPaths.
    void popImportPaths (Interpreter& interpretor,
			 const std::string& cmdLine,
			 std::istringstream& cmdArg,
			 std::ostream& os);

  } // end of namespace command.
} // end of namespace sot.

#endif //! SOT_FACTORY_COMMAND_IMPORT_H
