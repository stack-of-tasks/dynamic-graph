 /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      TimeDependancy.t.cpp
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




#ifndef __TIME_DEPENDANCY_TCPP
#define __TIME_DEPENDANCY_TCPP

#include <dynamic-graph/time-dependency.h>

namespace dynamicgraph {

#undef VP_TEMPLATE_DEBUG_MODE
#define VP_TEMPLATE_DEBUG_MODE 0
#include <dynamic-graph/debug.h>

#define __TIME_DEPENDANCY_INIT(sig,dep)     \
    leader(*sig)                                \
    ,dependancies()                             \
    ,updateFromAllChildren(ALL_READY_DEFAULT)   \
    ,dependancyType(dep)                        \
    ,periodTime(PERIOD_TIME_DEFAULT)
                         

template< class Time >
TimeDependancy<Time>::
TimeDependancy( SignalBase<Time> *sig
		   ,const DependancyType dep )
  :__TIME_DEPENDANCY_INIT(sig,dep)
{}

template< class Time >
TimeDependancy<Time>::
TimeDependancy( SignalBase<Time> * sig
		   ,const SignalArray_const<Time>& ar
		   ,const DependancyType dep )
  :__TIME_DEPENDANCY_INIT(sig,dep)
{
  for( unsigned int i=0;i<ar.getSize();++i ) 
    {addDependancy( ar[i] );  }
  
  return ;
}

/* -------------------------------------------------------------------------- */
template< class Time >
void TimeDependancy<Time>::
addDependancy( const SignalBase<Time>& sig )
{
  dependancies.push_front(&sig); 
}

template< class Time >
void TimeDependancy<Time>::
removeDependancy( const SignalBase<Time>& sig )
{
  dependancies.remove(&sig); 
}
template< class Time >
void TimeDependancy<Time>::
clearDependancy( void )
{
  dependancies.clear(); 
}

template< class Time >
bool TimeDependancy<Time>::
needUpdate( const Time& t1 ) const
{ 
  dgTDEBUG(15) << "# In {" << leader.getName() << " : ("<< leader.getReady()<<") "
		<< t1 << " >? " << leader.getTime() << std::endl;

  if( leader.getReady() ) { dgTDEBUGOUT(15);return true; }
  if( lastAskForUpdate ) { dgTDEBUGOUT(15);return true; }

  switch( dependancyType )
    {
    case ALWAYS_READY: 
      { dgTDEBUGOUT(15);return true; }
    case BOOL_DEPENDANT:  break;
    case TIME_DEPENDANT: 
      {
	if( t1<leader.getTime()+periodTime ) 
	  { dgTDEBUGOUT(15);return false; }
	break;
      }
    };

  bool res = updateFromAllChildren;
  const typename Dependancies::const_iterator itend=dependancies.end();
  for( typename Dependancies::const_iterator it=dependancies.begin();it!=itend;++it )
    {
      const SignalBase<Time> &sig = **it;
      dgTDEBUG(15)<< "Ask update for "<< sig <<std::endl;

      if( (sig.getTime()>leader.getTime()) || (sig.needUpdate(t1)) )
	{ if(updateFromAllChildren) continue; else {res=true; break;} }
      else { if(updateFromAllChildren) {res=false; break;} else continue; }
    }
  lastAskForUpdate = res;

  dgTDEBUGOUT(15);
  return res;
}


template< class Time >
std::ostream& TimeDependancy<Time>::
writeGraph(  std::ostream & os) const
{
  std::string LeaderLocalName;
  std::string LeaderNodeName;
  leader.ExtractNodeAndLocalNames(LeaderLocalName,LeaderNodeName);
  if (dependancies.size()!=0)
    {
      const typename Dependancies::const_iterator itend=dependancies.end();
      for( typename Dependancies::const_iterator it=dependancies.begin();it!=itend;++it )
	{
	  std::string itLocalName,itNodeName;
	  (*it)->ExtractNodeAndLocalNames(itLocalName,itNodeName);
	  os << "\"" << itNodeName << "\" -> \"" << LeaderNodeName << "\"" << std::endl
	     << " [ headlabel = \"" << LeaderLocalName << "\" , taillabel = \"" << itLocalName << "\" ]" << std::endl;
	}
    }
  return os;
}

template< class Time >
std::ostream& TimeDependancy<Time>::
displayDependancies( std::ostream& os,const int depth,
		     std::string space,
		     std::string next1,std::string next2 ) const
{
  leader.SignalBase<Time>::displayDependancies(os,depth,space,next1,next2)<<" (";
  switch( dependancyType )
    {
    case ALWAYS_READY: os<<"A"; break;
    case BOOL_DEPENDANT: os << "ready=" << ((leader.getReady())?"TRUE":"FALSE"); break;
    case TIME_DEPENDANT:  
      os  <<"t="<<leader.getTime() <<" (/"<<periodTime<<") " ; 
      break;
    };
  os<<")"; //<<std::endl;
    {
      const typename Dependancies::const_iterator itend=dependancies.end();
      for( typename Dependancies::const_iterator it=dependancies.begin();it!=itend;++it )
	if( depth!=0 )
	  {
	    os<<std::endl;
	    std::string ajout = "|";
	    std::string ajout2 = "|";
	    typename Dependancies::const_iterator it2=it; it2++;
	    if( it2==dependancies.end() ) { ajout = "`"; ajout2= " "; }
	    (*it)->displayDependancies( os,depth-1,space+next2+"   ",ajout,ajout2 );
	  }
	else 
	  { os<<std::endl<<space<<"   `-- ..."; break; }
    }
  return os;
}

} // namespace dynamicgraph

#endif /* #ifndef __TIME_DEPENDANCY_TCPP */


