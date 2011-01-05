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

#include <sstream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <dynamic-graph/entity.h>
#include <dynamic-graph/plugin-loader.h>
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/debug.h>

extern std::ofstream debugfile;

// Factorize exception catching code.
#define CATCH_EXCEPTIONS()						\
  catch (std::exception& e)						\
    {									\
      std::cout								\
	<< errorPrefix							\
	<< e.what () << std::endl;					\
      if (options.noninteractive)					\
	return 1;							\
    }									\
  catch (const char* str)						\
    {									\
      std::cout << errorPrefix						\
		<< "Unknown exception " << str << std::endl;		\
      if (options.noninteractive)					\
	return 1;							\
    }									\
  catch (...)								\
    {									\
      dgDEBUG(5) << errorPrefix << " Unknown exception " << std::endl;	\
      if (options.noninteractive)					\
	return 1;							\
    }									\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n

struct Options
{
  /// \brief Should we provide a shell to the user or end the program
  /// directly?
  bool noninteractive;
  /// \brief Prologue path (a prologue is a script implicitly
  /// evaluated at start-up).
  std::string prologue;
  /// \brief Files to be evaluated at start-up (left to right order).
  std::vector<std::string> inputs;
};

std::string defaultPrologue ()
{
  char* home = getenv ("HOME");
  boost::filesystem::path path (home);
  path/= ".dg-shellrc";
  return path.file_string ();
}

int main (int argc, char** argv)
{
  dgDEBUGIN(15);

  dgDEBUG(5) << " Loading..." << std::endl;
  dynamicgraph::PluginLoader pl;
  dynamicgraph::g_shell.referencePluginLoader (&pl);

  Options options;

  // Parse options.
  namespace po = boost::program_options;
  po::options_description desc ("Allowed options");
  desc.add_options ()
    ("input,i",
     po::value<std::vector<std::string> >(&options.inputs),
     "file(s) evaluated at start-up")
    ("noninteractive,n",
     "enable non-interactive behavior")
    ("prologue,p",
     po::value<std::string>(&options.prologue)->default_value (defaultPrologue ()),
     "override default prologue")
    ("help,h", "produce help message")
    ;

  po::variables_map vm;
  try
    {
      po::store (po::parse_command_line (argc, argv, desc), vm);
      po::notify (vm); 

      if (vm.count ("help"))
	{
	  std::cout << "Usage: " << argv[0] << " [options]" << std::endl
		    << desc << std::endl
		    << "Report bugs to <hpp@laas.fr>" << std::endl;
	  return 1;
	}
      options.noninteractive = vm.count ("noninteractive");
    }
  catch (po::error& error)
    {
      std::cerr << "Error while parsing argument: "
		<< error.what () << std::endl;
      return 1;
    }

  std::list<std::string> inputs (options.inputs.begin (),
				 options.inputs.end ());

  // First, evaluate the prologue.
  boost::filesystem::path prologuePath (options.prologue);
  if (!prologuePath.empty ())
    {
      if (boost::filesystem::exists (prologuePath)
	  && !boost::filesystem::is_directory (prologuePath))
	inputs.push_front (prologuePath.file_string ());
      else
	{
	  boost::format fmt ("Failed to load prologue: \"%s\".");
	  fmt % prologuePath.file_string ();
	  std::cerr << fmt.str () << std::endl;
	}
    }

  // Load all input files.
  BOOST_FOREACH (const std::string& pathStr, inputs)
    {
      boost::filesystem::path path (pathStr);

      std::stringstream ss;
      ss << "!! In file <" << path.file_string () << "> : ";
      std::string errorPrefix = ss.str ();

      try
	{
	  std::istringstream script (path.file_string ());
	  std::cout << "Run " << path.file_string () << std::endl;
	  dynamicgraph::g_shell.cmd ("run", script, std::cout);
	}
      CATCH_EXCEPTIONS ();
    }

  // Interactive loop.
  if (options.noninteractive)
    return 0;
  std::string errorPrefix = "";
  while (1)
    {
      try
	{
	  dgDEBUG(5) << "Run shell." << std::endl;
	  dynamicgraph::g_shell.shell (std::cin, std::cout);
	  dgDEBUG(5) << "Shell over." << std::endl;
	  if (std::cin.eof ())
	    break;
	}
      CATCH_EXCEPTIONS ();
    }

  dgDEBUGOUT(15);
  return 0;
}
