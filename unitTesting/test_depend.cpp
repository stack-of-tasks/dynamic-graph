/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet VISTA / IRISA, 2003
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      test_categorie.cc
 * Project:   Traces
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id: test_boost.cpp,v 1.1.1.1 2006-07-03 05:17:37 nmansard Exp $
 *
 * Description
 * ============
 *
 * Test la classe CategorieTrace.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* -------------------------------------------------------------------------- */
/* --- INCLUDES ------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
// #include <dynamic-graph/all-signals.h>
#include <dynamic-graph/signal.h>
#include <dynamic-graph/signal-time-dependent.h>
//#include <sot/TimeDependency.h>
#include <dynamic-graph/debug.h>
#include <iostream>
#include <string>

using namespace std;
using namespace dynamicgraph;



template< class Res=double >
class DummyClass
{

public:
  std::string proname;
  list< SignalTimeDependent<double,int>* > inputsig;
  list< SignalTimeDependent<string,int>* > inputsigV;

public:
  DummyClass( const std::string& n ) : proname(n),res(),appel(0),timedata(0) {}

  Res& fun( Res& res,int t) 
  {
    appel++;  timedata=t; 

    cout << "Inside " << proname << " -> " << this 
	 << endl;
    for( list< SignalTimeDependent<double,int>* >::iterator it=inputsig.begin();
	 it!=inputsig.end();++it )
      {
	cout << *(*it) << endl; 
	(*it)->access(timedata);
      }
    for( list< SignalTimeDependent<string,int>* >::iterator it=inputsigV.begin();
	 it!=inputsigV.end();++it )
      { cout << *(*it) << endl; (*it)->access(timedata);}

    return res=(*this)();
  }

  void add( SignalTimeDependent<double,int>& sig )
  {    inputsig.push_back(&sig);   }
  void add( SignalTimeDependent<string,int>& sig )
  {    inputsigV.push_back(&sig);   }

  Res operator() ( void );

  Res res;
  int appel;
  int timedata;
  
};

template< class Res >
Res DummyClass<Res>::operator() (void)
{ return this->res; }

template<>
double DummyClass<double>::operator() (void)
{
  res=appel*timedata; return res;
}
template<>
string DummyClass<string>::operator() (void)
{
  ostringstream oss;
  oss << appel*timedata;
  return oss.str();
}


int main( void )
{
   DummyClass<double> pro1("pro1"),pro3("pro3"),pro5("pro5");
   DummyClass<string> pro2("pro2"),pro4("pro4"),pro6("pro6");

   SignalTimeDependent<double,int> sig5("Sig5");
   SignalTimeDependent<string,int> sig6("Sig6");
   
   SignalTimeDependent<string,int> sig4(sig5,"Sig4");
   SignalTimeDependent<string,int> sig2(sig4<<sig4<<sig4<<sig6,"Sig2");
   SignalTimeDependent<double,int> sig3(sig2<<sig5<<sig6,"Sig3");
   SignalTimeDependent<double,int> sig1( boost::bind(&DummyClass<double>::fun,&pro1,_1,_2),
					   sig2<<sig3,"Sig1");

//    cout << "--- Test Array ------ "<<endl;
//    SignalArray<int> tarr(12);
//    tarr<<sig3<<sig2;//+sig2+sig3;
//    dispArray(sig4<<sig2<<sig3);
//    dispArray(tarr);

   sig2.setFunction( boost::bind(&DummyClass<string>::fun,&pro2,_1,_2) );
   sig3.setFunction( boost::bind(&DummyClass<double>::fun,&pro3,_1,_2) );
   sig4.setFunction( boost::bind(&DummyClass<string>::fun,&pro4,_1,_2) );
   sig5.setFunction( boost::bind(&DummyClass<double>::fun,&pro5,_1,_2) );
   sig6.setFunction( boost::bind(&DummyClass<string>::fun,&pro6,_1,_2) );

   pro1.add(sig2);
   pro1.add(sig3);
   pro2.add(sig4);
   pro2.add(sig4);
   pro2.add(sig4);
   pro3.add(sig2);
   pro4.add(sig5);
   pro2.add(sig6);
   pro3.add(sig5);
   pro3.add(sig6);
   
   sig5.setDependencyType(TimeDependency<int>::ALWAYS_READY);
   sig6.setDependencyType(TimeDependency<int>::BOOL_DEPENDENT);

   sig6.setReady();

   sig1.displayDependencies(cout)<<endl;
 
   cout << "Needs update?"    << endl 
	<< sig1.needUpdate(2) << endl;
   dgDEBUG(1) << "Access sig1(2) "<<endl;
   sig1.access(2);
   sig1.displayDependencies(cout) << endl;
   dgDEBUG(1) << "Access sig2(4) "<<endl;
   sig2.access(4);
   sig1.displayDependencies(cout)<<endl;
   dgDEBUG(1) << "Access sig1(4) "<<endl;
   sig1.access(4);
   sig1.displayDependencies(cout)<<endl;

   sig1.needUpdate(6);
   sig1.needUpdate(6);

  return 0;

}
