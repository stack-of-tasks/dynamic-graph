/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      sotContiifstream.cpp
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

#include <dynamicGraph/contiifstream.h>
#include <dynamicGraph/debug.h>

sotContiifstream::
sotContiifstream( const std::string& n )
  :filename(n),cursor(0),first(true) {}


sotContiifstream::
~sotContiifstream( void )
{
  dgDEBUGINOUT(5);
}


bool sotContiifstream::
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
sotContiifstream::next( void ) 
{
  std::string res = *reader.begin();
  reader.pop_front();
  return res;
}





