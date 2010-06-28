/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      Contiifstream.cpp
 * Project:   DYNAMIC-GRAPH
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

#include <dynamic-graph/contiifstream.h>
#include <dynamic-graph/debug.h>

using namespace dynamicgraph;

Contiifstream::
Contiifstream( const std::string& n )
  :filename(n),cursor(0),first(true) {}


Contiifstream::
~Contiifstream( void )
{
  dgDEBUGINOUT(5);
}


bool Contiifstream::
loop( void )
{
  dgDEBUGIN(25);
  bool res=false;
  
  std::fstream file( filename.c_str() );

  file.seekg(cursor);
  file.sync();
      
  while(1)
    {
      file.get(buffer,BUFFER_SIZE);
      if( file.gcount() ) 
	{ 
	  res=true;
	  std::string line(buffer);
	  if(! first) reader.push_back(line);
	  cursor=file.tellg(); cursor++;
	  file.get(*buffer); // get the last char ( = '\n')
	  dgDEBUG(15) << "line: "<< line<<std::endl;
	}
      else { break; }
    }

  first=false;
  dgDEBUGOUT(25);
  return res;
}

std::string 
Contiifstream::next( void ) 
{
  std::string res = *reader.begin();
  reader.pop_front();
  return res;
}





