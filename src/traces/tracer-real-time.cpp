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

/* DG */
#include <iomanip>
#include <boost/bind.hpp>

#include <dynamic-graph/tracer-real-time.h>
#include <dynamic-graph/debug.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/all-commands.h>

using namespace std;
using namespace dynamicgraph;

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(TracerRealTime,"TracerRealTime");

/* --------------------------------------------------------------------- */
/* --- DGOUTSTRINGSTREAM ---------------------------------------------- */
/* --------------------------------------------------------------------- */

OutStringStream::
OutStringStream  ()
  : std::ostringstream ()
    ,buffer( 0 ),index(0),bufferSize(0),full(false)
{
  dgDEBUGINOUT(15);
}

OutStringStream::
~OutStringStream  ()
{
  dgDEBUGIN(15);
  delete[] buffer;
  dgDEBUGOUT(15);
}

void OutStringStream::
resize (const std::streamsize& size)
{
  dgDEBUGIN(15);

  index=0;
  bufferSize = size;
  full=false;

  delete[] buffer;
  buffer = new char[static_cast<size_t> (size)];

  dgDEBUGOUT(15);
}

bool OutStringStream::
addData (const char * data, const std::streamoff& size)
{
  dgDEBUGIN(15);
  std::streamsize towrite = static_cast<std::streamsize> (size);
  if (index + towrite > bufferSize)
    {
      dgDEBUGOUT(15);
      full = true;
      return false;
    }
  memcpy (buffer + index, data, static_cast<size_t> (towrite));
  index += towrite;
  dgDEBUGOUT(15);
  return true;
}

void OutStringStream::
dump( std::ostream& os )
{
  dgDEBUGIN(15);
  os.write( buffer,index );
  dgDEBUGOUT(15);
}

void OutStringStream::
empty  ()
{
  dgDEBUGIN(15);
  index=0; full=false;
  dgDEBUGOUT(15);
}



/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */


TracerRealTime::TracerRealTime( const std::string n )
  :Tracer(n)
   ,bufferSize( BUFFER_SIZE_DEFAULT )
{
  dgDEBUGINOUT(15);

  /* --- Commands --- */
  {
    using namespace dynamicgraph::command;
    std::string doc
      = docCommandVoid0("Trash the current content of the buffers, without saving it.");
    addCommand("empty",
	       makeCommandVoid0(*this,&TracerRealTime::emptyBuffers,doc ));

    addCommand("getBufferSize",
	       makeDirectGetter(*this,&bufferSize,
				docDirectGetter("bufferSize","int")));
    addCommand("setBufferSize",
	       makeDirectSetter(*this,&bufferSize,
				docDirectSetter("bufferSize","int")));
  } // using namespace command

  dgDEBUGOUT(15);
}

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */


void TracerRealTime::
openFile( const SignalBase<int> & sig,
	  const std::string& givenname )
{
  dgDEBUGIN(15);
  string signame;
  if( givenname.length () )
    { signame = givenname;  } else { signame = sig.shortName (); }

  string filename = rootdir + basename + signame + suffix;
  dgDEBUG(5) << "Sig <"<<sig.getName ()
	      << ">: new file "<< filename << endl;
  std::ofstream * newfile = new std::ofstream( filename.c_str () );
  dgDEBUG(5) << "Newfile:" << (void*) newfile << endl;
  hardFiles.push_back( newfile );
  dgDEBUG(5) << "Creating Outstringstream" << endl;

  //std::stringstream * newbuffer = new std::stringstream ();
  OutStringStream * newbuffer = new OutStringStream (); // std::stringstream ();
  newbuffer->resize( bufferSize );
  newbuffer->givenname = givenname;
  files.push_back( newbuffer );

  dgDEBUGOUT(15);
}


void TracerRealTime::
closeFiles  ()
{
  dgDEBUGIN(15);

  FileList::iterator iter = files.begin ();
  HardFileList::iterator hardIter = hardFiles.begin ();

  while( files.end ()!=iter )
    {
      dgDEBUG(25) << "Close the files." << endl;

      std::stringstream * file = dynamic_cast< stringstream* >(*iter);
      std::ofstream * hardFile = *hardIter;

      (*hardFile) <<flush; hardFile->close ();
      delete file;
      delete hardFile;

      ++iter; ++hardIter;
    }

  dgDEBUG(25) << "Clear the lists." << endl;
  files.clear ();
  hardFiles.clear ();

  dgDEBUGOUT(15);
}

void TracerRealTime::
trace  ()
{
  dgDEBUGIN(15);

  FileList::iterator iter = files.begin ();
  HardFileList::iterator hardIter = hardFiles.begin ();

  while( files.end ()!=iter )
    {
      dgDEBUG(35) << "Next" << endl;
      std::ostream * os = *iter;
      if( NULL==os )
	{ DG_THROW ExceptionTraces( ExceptionTraces::NOT_OPEN,
					"The buffer is null",""); }
      //std::stringstream & file = * dynamic_cast< stringstream* >(os);
      OutStringStream * file = dynamic_cast< OutStringStream* >(os); // segfault
      if( NULL==file )
	{ DG_THROW ExceptionTraces( ExceptionTraces::NOT_OPEN,
					"The buffer is not open",""); }

      std::ofstream & hardFile = **hardIter;
      if(! hardFile.good () )
	{ DG_THROW ExceptionTraces( ExceptionTraces::NOT_OPEN,
					"The file is not open",""); }

      if( (hardFile.good ())&&(NULL!=file) )
	{

// 	  const unsigned int SIZE = 1024*8;
// 	  char buffer[SIZE];
// 	  streambuf * pbuf = file.rdbuf ();
// 	  pbuf->pubseekpos(0);
// 	  const unsigned int NB_BYTE = pbuf->in_avail ();
// 	  dgDEBUG(35) << "Bytes in buffer: " << NB_BYTE << endl;
// 	  //dgDEBUG(35) << "Copie" <<endl<<file.str ()<< endl;

// 	  for( unsigned int index=0;index<NB_BYTE;index+=SIZE )
// 	    {
// 	      pbuf->pubseekpos( index );
// 	      int nget = pbuf->sgetn( buffer,SIZE );
// 	      dgDEBUG(35) << "Copie ["<<nget<<"] " <<buffer<<endl;
// 	      hardFile.write( buffer,nget );
// 	    }
	  //hardFile << file.str () << flush;
	  //file.seekp(0);

	  file->dump( hardFile );
	  file->empty ();
	  hardFile.flush ();

	  //file.str("");
	}

      ++iter; ++hardIter;
    }

  dgDEBUGOUT(15);
}

void TracerRealTime::
emptyBuffers  ()
{
  dgDEBUGIN(15);
  for( FileList::iterator iter = files.begin ();files.end ()!=iter;++iter )
    {
      //std::stringstream & file = * dynamic_cast< stringstream* >(*iter);
      try {
	OutStringStream & file  = * dynamic_cast< OutStringStream* >(*iter);
	file.empty ();
	//file.str("");
      }
      catch( ... ) { DG_THROW ExceptionTraces( ExceptionTraces::NOT_OPEN,
						   "The buffer is not open",""); }
    }
  dgDEBUGOUT(15);
}


// void TracerRealTime::
// emptyBuffer( std::stringstream & file )
// {
//   streambuf * pbuf = file.rdbuf ();
//   pbuf->file.rdbuf () ->pubsetbuf( fileBuffer,10 );



// }
void TracerRealTime::
recordSignal( std::ostream& os,
	      const SignalBase<int>& sig )
{
  dgDEBUGIN(15);

  try {

    OutStringStream & file = dynamic_cast< OutStringStream& >(os);
    file.str("");
    dgDEBUG(45) << "Empty file [" << file.tellp ()
		 << "] <" << file.str ().c_str () <<"> " <<endl;

    Tracer::recordSignal( file,sig );
    file.addData( file.str ().c_str (),file.tellp () );
    dgDEBUG(35) << "Write data [" << file.tellp ()
		 << "] <" << file.str ().c_str () <<"> " <<endl;

  } catch( ExceptionAbstract & exc ) { throw exc; }
  catch( ... ) {
    DG_THROW ExceptionTraces( ExceptionTraces::NOT_OPEN,
				  "The buffer is not open","");
  }


  dgDEBUGOUT(15);
  return ;
}



/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

void TracerRealTime::
display( std::ostream& os ) const
{
  os << CLASS_NAME << " " << name << " [mode=" << (play?"play":"pause")
     << "] : "<< endl
     << "  - Dep list: "<<endl;

  FileList::const_iterator iterFile = files.begin ();
  for( SignalList::const_iterator iter = toTraceSignals.begin ();
       toTraceSignals.end ()!=iter;++iter )
    {
      dgDEBUG(35) << "Next" << endl;
      const OutStringStream * file = dynamic_cast< OutStringStream* >(*iterFile);
      os << "     -> "<<(*iter)->getName ();
      if( file->givenname.length () ) os << " (in " << file->givenname << ")" ;
      os << "\t";
      if( file )
	{
	  const std::streamsize PRECISION = os.precision ();
	  const std::streamsize SIZE = file->index;
	  const std::streamsize MSIZE = file->bufferSize;
	  unsigned int dec=0; std::string unit ="";
	  if( (SIZE>>30)||(MSIZE>>30) ) { dec = 30; unit="Go"; }
	  else if( (SIZE>>20)||(MSIZE>>20) ) { dec = 20; unit="Mo"; }
	  else if( (SIZE>>10)||(MSIZE>>10) ) { dec = 10; unit="Ko"; }
 	  os << "[" << std::setw(1)<<std::setprecision(1)
 	     << ((SIZE+0.0)/(1<<dec)) << unit << "/"
 	     << std::setprecision(2)<<((MSIZE+0.0)/(1<<dec))
 	     << unit << "]\t";
	  if( file->full ) os << "(FULL)";
	  os.precision(PRECISION);
	}
      os<<endl;
      iterFile++;
    }

}


std::ostream& operator<< ( std::ostream& os,const TracerRealTime& t )
{
  t.display(os);
  return os;
}


void TracerRealTime::
commandLine( const std::string& cmdLine
	     ,std::istringstream& cmdArgs
	     ,std::ostream& os )
{
  if( cmdLine=="help" )
    {
      os << "TracerRealTime: "<<endl
	 << "  - empty: trash the buffers." <<endl
	 << "  - bufferSize [<size>]: get/set the buffer size." <<endl;

      Tracer::commandLine( cmdLine,cmdArgs,os );
    }
  else if( cmdLine=="empty" )
    {
      emptyBuffers ();
    }
  else if( cmdLine=="bufferSize" )
    {
      cmdArgs>>ws;
      if( cmdArgs.good () )
	{
	  int s; cmdArgs>>s;
	  setBufferSize( s );
	}
      else os << getBufferSize () << endl;
    }


  else
    Tracer::commandLine( cmdLine,cmdArgs,os );



}
