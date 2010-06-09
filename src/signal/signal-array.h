/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      SignalArray.h
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




#ifndef __SIGNAL_ARRAY_HH
#define __SIGNAL_ARRAY_HH

#include <dynamic-graph/signal-base.h>
#include <dynamic-graph/dynamicGraph-API.h>

namespace dynamicgraph {

template<class Time>
class SignalArray;

template<class Time>
class SignalArray_const
{ 
 public:
  static const int DEFAULT_SIZE = 20; 

 protected:
  const SignalBase<Time> ** const_array;
  unsigned int size,rank; 

 public:
  SignalArray_const<Time>( const unsigned int& sizeARG=DEFAULT_SIZE ) 
    : const_array(NULL),size(sizeARG),rank(0)
    { createArray(); }

  SignalArray_const<Time>( const SignalBase<Time>& sig) 
    : const_array(NULL),size(1),rank(0)
    { createArray(); addElmt(&sig); }

   SignalArray_const<Time> ( const SignalArray<Time>& siga )
    : const_array(NULL),size(DEFAULT_SIZE),rank(siga.getSize())
    {
      createArray();
      for( unsigned int i=0;i<rank;++i ) const_array[i]=&siga[i];
    }

   SignalArray_const<Time> ( const SignalArray_const<Time>& siga )
    : const_array(NULL),size(DEFAULT_SIZE),rank(siga.getSize())
    {
      createArray();
      for( unsigned int i=0;i<rank;++i ) const_array[i]=&siga[i];
    }

  virtual ~SignalArray_const<Time>( void )
    {
      if( NULL!=const_array ) delete [] const_array;
    }

 protected:
  void createArray( void )
    {
      if(0<size) {  const_array = new const SignalBase<Time>* [size]; }
    }
  void addElmt( const SignalBase<Time>* el ) 
    { if(rank<size) const_array[rank++] = el;  }

 public: 
  virtual SignalArray_const<Time>& operator<< (const SignalBase<Time>& sig)
    { addElmt( &sig ); return *this; }

 public:
  virtual const SignalBase<Time>& operator[] ( const unsigned int& idx ) const
    { return *const_array[idx];   }
  virtual unsigned int getSize( void ) const { return rank; }

/*   SignalArray_const<Time>& operator= ( SignalArray<Time> & siga ) */
/*     { */
/*       for( unsigned int i=0;i<rank;++i ) const_array[i]=&siga[i];  */
/*       rank=siga.getSize(); */
/*     } */
    

  //  friend class SignalArray<Time>;
};


template<class Time>
SignalArray_const<Time> operator<<( const SignalBase<Time>& sig1,
				       const SignalBase<Time>& sig2 )
{ SignalArray_const<Time> res(20); res<<sig1; res<<sig2; return res;}


/* ---- */


template<class Time>
class SignalArray
:public SignalArray_const<Time>
{ 
 public:
  using SignalArray_const<Time>::DEFAULT_SIZE;
  using SignalArray_const<Time>::size;
  using SignalArray_const<Time>::rank;
 protected:
  mutable SignalBase<Time> ** array;

 public:
  SignalArray<Time>( const unsigned int& sizeARG=DEFAULT_SIZE ) 
    :SignalArray_const<Time>(0)
    { size=sizeARG; createArray(); }

  SignalArray<Time>( SignalBase<Time>& sig) 
    : SignalArray_const<Time>(0)
    { size=1; createArray(); addElmt(&sig); }

   SignalArray<Time> ( const SignalArray<Time>& siga )
     :SignalArray_const<Time>( DEFAULT_SIZE )
     ,array(NULL)
    {
      rank = siga.getSize();
      createArray();
      for( unsigned int i=0;i<rank;++i ) array[i]=&siga[i];
    }
  virtual ~SignalArray<Time>( void )
    {
      if( NULL!=array ) delete [] array;
    }

 protected:
  void createArray( void )
    {
      if(0<size) {  array = new SignalBase<Time>* [size]; }
    }
  void addElmt( SignalBase<Time>* el ) 
    { if(rank<size) array[rank++] = el;  }

 public: //protected:
  virtual SignalArray<Time>& operator<< (SignalBase<Time>& sig)
    { addElmt( &sig ); return *this; }

  virtual SignalArray_const<Time> operator<< (const SignalBase<Time>& sig) const
    {
      SignalArray_const<Time> res(size); 
      res=*this; 
      res<<sig; 
      return res; 
    }


 public:
  virtual SignalBase<Time>& operator[] ( const unsigned int& idx ) const
    { 
      return *array[idx];
    }
};




template<class Time>
SignalArray<Time> operator<<( SignalBase<Time>& sig1,
				 SignalBase<Time>& sig2 )
{ SignalArray<Time> res(20); res<<sig1; res<<sig2; return res;}

	DYNAMICGRAPH_EXPORT extern SignalArray<int> sotNOSIGNAL;

} // namespace dynamicgraph

#endif /* #ifndef __SIGNAL_ARRAY_HH */
