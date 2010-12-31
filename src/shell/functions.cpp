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

#include <fstream>

#ifdef WIN32
# include <Windows.h>
#endif // WIN32

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/functions.h>
#include <dynamic-graph/interpreter.h>
#include <dynamic-graph/plugin-loader.h>
#include <dynamic-graph/signal.h>

using namespace dynamicgraph;

static const char* NO_PLUGINLOADER_ERROR_MSG =
  "!!  Dynamic loading functionalities not accessible through the shell.";

static void sleep (int secs)
{
#ifndef WIN32
  usleep(secs * 1000000);
#else
  Sleep (secs * 1000);
#endif //! WIN32
}



void
ShellFunctions::cmdTry (const std::string cmdLine,
			std::istringstream& cmdArg,
			std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - try <cmd...>"
	 << "\t\t\t\tTry <cmd> and display the caught exception." << std::endl;
      return;
    }

  std::string cmdLine2;
  cmdArg >> cmdLine2;
  dgDEBUG(5) << "Try <" << cmdLine2 << ">" << std::endl;

  try
    {
      g_shell.cmd (cmdLine2,cmdArg,os);
    }
  catch (const ExceptionAbstract& e)
    {
      os << "dgERROR catch: " <<std::endl
	 << e << std::endl;
    }
  catch(...)
    {
      os << "Unknown error caught." << std::endl;
    }
}

void
ShellFunctions::cmdLoadPlugins (const std::string cmdLine,
				std::istringstream& cmdArg,
				std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - loadPlugins <file.txt> <directory>"
	 << "\t\tLoad the plugins listed in the file." << std::endl;
      return;
    }

  if (!g_shell.dlPtr)
    {
      os << NO_PLUGINLOADER_ERROR_MSG << std::endl;
      return;
    }

  std::string pluginName, directory;
  cmdArg >> pluginName;
  cmdArg >> directory;

  dgDEBUG(15) << "Load plugin list <"
	      << pluginName << "> from dir <" << directory
	      << ">." << std::endl;

  if (directory.length () != 0)
    g_shell.dlPtr->setDirectory (directory);

  g_shell.dlPtr->loadPluginList (pluginName);
  g_shell.dlPtr->loadPlugins ();
}

void
ShellFunctions::cmdClearPlugin (const std::string cmdLine,
				std::istringstream& cmdArg,
				std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - clearPlugin <className>"
	 << "\t\tDestroy all the objects of type <className>." <<std::endl;
      return;
    }

  std::string pluginName;
  cmdArg >> pluginName;
  g_pool.clearPlugin (pluginName);
}

void
ShellFunctions::cmdDisplayPlugins (const std::string cmdLine,
				   std::istringstream&,
				   std::ostream& os)
{
  typedef std::pair< std::string,std::string > plugin_t;

  if (cmdLine == "help")
    {
      os << "  - displayPlugins "
	 << "\t\t\t\tDisplay the name of the loaded plugins." <<std::endl;
      return;
    }

  if (!g_shell.dlPtr)
    {
      os << NO_PLUGINLOADER_ERROR_MSG << std::endl;
      return;
    }

  boost::format fmt ("- <%s>:\t %s");
  BOOST_FOREACH(plugin_t plugin, g_shell.dlPtr->getLoadedPluginNames ())
    {
      fmt % plugin.first % plugin.second;
      os << fmt.str () << std::endl;
    }
}


void
ShellFunctions::cmdDisplayFactory (const std::string cmdLine,
				   std::istringstream& cmdArg,
				   std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - ";
      g_factory.commandLine(cmdLine,cmdArg,os);
      return;
    }

  std::string cmd2;
  cmdArg >> cmd2;
  g_factory.commandLine (cmd2, cmdArg, os);
}

void
ShellFunctions::cmdCommentary (const std::string cmdLine,
			       std::istringstream&,
			       std::ostream& os)
{
  if (cmdLine == "help")
    os << "  # comment with '#': ignore the end of the line." << std::endl;
  return;
}


void
ShellFunctions::cmdUnplug (const std::string cmdLine,
			   std::istringstream& cmdArg,
			   std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - unplug <obj2.sig2>"
	 << "\t\t\t\tPlug on sig2 (consumer) in sig1->sig2." << std::endl;
      return;
    }

  std::string ssig;
  cmdArg>>ssig;

  std::string obj2, fun2;
  std::istringstream str2 (ssig);

  if (!Interpreter::objectNameParser(str2, obj2, fun2))
    {
      DG_THROW ExceptionFactory
	(ExceptionFactory::SYNTAX_ERROR,
	 "Plug function: syntax is unplug OBJ2.SIG2.",
	 "(while calling plug %s %s).",ssig.c_str ());
    }

  dgDEBUG(20) << "Get Ent2 <" << obj2 << "> ." << std::endl;
  Entity& ent2 = g_pool.getEntity (obj2);
  dgDEBUG(20) << "Get Sig2 <" << fun2 << "> ." << std::endl;
  SignalBase<int>& sig2 = ent2.getSignal (fun2);

  dgDEBUG(25) << "Unplug..." << std::endl;
  sig2.unplug ();
}

void
ShellFunctions::cmdSignalTime (const std::string cmdLine,
			       std::istringstream& cmdArg,
			       std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - signalTime <obj.sig>"
	 << "\t\t\t\tDisplay the time of sig." << std::endl;
      return;
    }

  std::string ssig;
  cmdArg >> ssig;

  std::string obj2,fun2;
  std::istringstream str2 (ssig);
  if (!Interpreter::objectNameParser(str2,obj2,fun2))
    {
      DG_THROW ExceptionFactory
	(ExceptionFactory::SYNTAX_ERROR,
	 "signalTime function: syntax is signalTime OBJ2.SIG2.",
	 "(while calling signalTime %s).",ssig.c_str ());
    }

  dgDEBUG(20) << "Get Ent2 <" << obj2 << "> ." << std::endl;
  Entity& ent2 = g_pool.getEntity (obj2);
  dgDEBUG(20) << "Get Sig2 <" << fun2 << "> ." << std::endl;
  SignalBase<int>& sig2 = ent2.getSignal (fun2);

  os << sig2.getTime () << std::endl;
}

void
ShellFunctions::cmdSynchroSignal (const std::string cmdLine,
				  std::istringstream& cmdArg,
				  std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - synchro <obj.sig> [<period>]"
	 << "\t\t\t\tGet/Set the syncho of a signal <sig>." << std::endl;
      return;
    }

  std::string ws;

  SignalBase<int>& sig = g_pool.getSignal (cmdArg);
  cmdArg >> ws;

  if (!cmdArg.good ())
    {
      os << "period = " << sig.getPeriodTime () << std::endl;
      return;
    }

  int period = 0;
  cmdArg >> period;
  sig.setPeriodTime (period);
  cmdArg >> ws;
  if (cmdArg.good ())
    {
      /* The sig is recomputed at the given period from the
       * current time, at any time T so that T%p==0, p the period.
       * By modifying the current time, the sig reomputation is done
       * at T s.t. T%p=d, d the desynchro. */
      int currTime = sig.getTime ();
      int desynchro;
      cmdArg>>desynchro;
      sig.setTime (currTime + desynchro);
    }
}

void
ShellFunctions::cmdEcho (const std::string cmdLine,
			 std::istringstream& cmdArg,
			 std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - echo <string>"
	 << "\t\t\t\tPrint <string. on the standard output." << std::endl;
      return;
    }

  std::string ws;
  cmdArg >> ws;
  while (cmdArg.good ())
    {
      std::string toPrint;
      cmdArg >> toPrint;
      os << toPrint << " ";
    }
  os << std::endl;
}

void
ShellFunctions::cmdCopy (const std::string cmdLine,
			 std::istringstream& cmdArg,
			 std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - copy <obj1.sig1> <obj2.sig2>"
	 << "\t\tCopy the value of sig1 to constant value in sig2."
	 << std::endl;
      return;
    }

  std::string ssig1, ssig2;
  cmdArg >> ssig1 >> ssig2;

  std::istringstream str1 (ssig1), str2 (ssig2);

  try
    {
      SignalBase<int>& sig1 = g_pool.getSignal (str1);
      SignalBase<int>& sig2 = g_pool.getSignal (str2);

      dgDEBUG(25) << "Copy..."<< std::endl;
      sig2.plug (&sig1);
      sig2.setConstantDefault ();
      sig2.plug (&sig2);
    }
  catch (ExceptionAbstract& err)
    {
      throw;
    }
  catch (...)
    {
      DG_THROW ExceptionFactory
	(ExceptionFactory::SYNTAX_ERROR,
	 "Copy: syntax is copy OBJ1.SIG1 OBJ2.SIG2.",
	 "(while calling copy %s %s).",ssig1.c_str (),
	 ssig2.c_str ());
    }
}

void
ShellFunctions::cmdFreeze (const std::string cmdLine,
			   std::istringstream& cmdArg,
			   std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - freeze <obj.sig> "
	 << "\t\tOn a ptr-sig: save the current value from the source "
	 << "and unplug the signal." << std::endl;
      return;
    }
  std::string ssig1;
  cmdArg >> ssig1;
  std::istringstream str1 (ssig1);

  try
    {
      SignalBase<int>& sig1 = g_pool.getSignal (str1);

      dgDEBUG(25) << "Unplug..."<< std::endl;
      sig1.setConstantDefault ();
      sig1.plug(&sig1);
    }
  catch (...)
    {
      DG_THROW ExceptionFactory
	(ExceptionFactory::SYNTAX_ERROR,
	 "Freeze: syntax is freeze OBJ.SIG.",
	 "(while calling freeze %s ).", ssig1.c_str ());
    }
}

void
ShellFunctions::cmdSqueeze (const std::string cmdLine,
			    std::istringstream& cmdArg,
			    std::ostream& os)
{
  if (cmdLine == "help")
    {
      os <<
	"  - squeeze  <mainObj.mainSig> <squeezeObj.sigIn> <squeezeObj.sigOut>"
	 << "\t\tIntercalate squeezeObj between mainObj and its source."
	 << std::endl;
      return;
    }

  std::string ssigMain, ssigIn, ssigOut;
  cmdArg >> ssigMain >> ssigIn >> ssigOut;
  std::istringstream strMain(ssigMain);
  std::istringstream strIn(ssigIn);
  std::istringstream strOut(ssigOut);

  try
    {
      SignalBase<int>& sigMain = g_pool.getSignal (strMain);
      SignalBase<int>& sigIn = g_pool.getSignal (strIn);
      SignalBase<int>& sigOut = g_pool.getSignal (strOut);

      SignalBase<int>* sigMainSource = sigMain.getPluged ();
      if(sigMainSource == &sigMain)
	{
	  DG_THROW ExceptionFactory
	    (ExceptionFactory::SYNTAX_ERROR,
	     "The main signal is autopluged (or set constant). ",
	     "(while calling freeze %s ).",ssigMain.c_str ());
	}

      sigMain.plug (&sigOut);
      sigIn.plug(sigMainSource);
    }
  catch (const ExceptionFactory& exc)
    {
      switch (exc.getCode ())
	{
	case ExceptionFactory::UNREFERED_SIGNAL:
	  DG_THROW ExceptionFactory
	    (ExceptionFactory::SYNTAX_ERROR,
	     "Sqeeze: unknown signal. ",
	     "(error while searching signal: %s ).",
	     exc.getMessage ());
	  break;
	default:
	  throw;
	}
    }
}


void
ShellFunctions::cmdEnableTrace (const std::string cmdLine,
				std::istringstream& cmdArg,
				std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - debugtrace [{true|false}] [<filename>="
	 << DebugTrace::DEBUG_FILENAME_DEFAULT <<"]"
	 << "\t\tOpen/close the file <filename> for debug tracing."
	 << std::endl;
      return;
    }

  std::string ws;
  cmdArg >> ws;
  if (!cmdArg.good ())
    {
      if (dgDEBUGFLOW.outputbuffer.good ())
	os << "true" << std::endl;
      else
	os << "false" << std::endl;
      return;
    }

  std::string opt;
  std::string filename;

  cmdArg >> opt >> ws;
  if (opt == "true")
    if (cmdArg.good ())
      {
	cmdArg >> filename;
	DebugTrace::openFile (filename.c_str ());
      }
    else
      DebugTrace::openFile ();
  else
    DebugTrace::closeFile ();
}

void
ShellFunctions::cmdSetPrompt (const std::string cmdLine,
			      std::istringstream& cmdArg,
			      std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - prompt [<string>] "
	 << "\t\tSet/get the default prompt." << std::endl;
      return;
    }

  std::string opt;
  std::string ws;
  cmdArg >> ws;

  if (!cmdArg. good ())
    {
      os << "Current prompt is <" << g_shell. prompt << ">." << std::endl;
      return;
    }

  char buffer [80];
  cmdArg.getline (buffer, 80);
  g_shell.prompt = buffer;
}

void
ShellFunctions::cmdSleep (const std::string cmdLine,
			  std::istringstream& cmdArg,
			  std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - sleep [<float> secs] "
	 << "\t\tSleep (time in secs)." << std::endl;
      return;
    }

  std::string opt;
  std::string ws;
  cmdArg >> ws;
  if (!cmdArg. good ())
    {
      //FIXME: handle error here.
      return;
    }

  double secs = 0.;
  cmdArg >> secs;
  if (secs > 0)
    sleep (int (secs));
}

void
ShellFunctions::cmdBeep (const std::string cmdLine,
			 std::istringstream&,
			 std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - beep [<float> secs] "
	 << "\t\tSend a bip to the std::cout." << std::endl;
      return;
    }

  os << char (7) << "Beep!" << std::endl;
}

void
ShellFunctions::cmdCompletionList (const std::string cmdLine,
				   std::istringstream& cmdArg,
				   std::ostream& os)
{
  if (cmdLine == "help")
    {
      os << "  - completion <filename>"
	 << "\t\tGenerate the completion list for current graph." << std::endl;
      return;
    }

  try
    {
      std::string aFileName;
      cmdArg >> aFileName;

      std::ofstream completionFile (aFileName.c_str ());
      g_pool.writeCompletionList (completionFile);
    }
  catch (const ExceptionAbstract & err)
    {
      throw;
    }
  catch (...)
    {
      DG_THROW ExceptionFactory
	(ExceptionFactory::SYNTAX_ERROR,
	 "setflag: sig should be of flag type. ",
	 "(while calling setflag).");
    }
}
