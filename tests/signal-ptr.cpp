// Copyright 2010 Thomas Moulard.
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dynamic-graph is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// You should have received a copy of the GNU Lesser General Public License
// along with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#include <string>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal.h>
#include <dynamic-graph/pool.h>
#include <iostream>


int main( void )
{
  using namespace dynamicgraph;
  using std::cout;

  Signal<double,int> sig("sig");
  SignalPtr<double,int> sigPtrA(NULL,"sigPtrA"),sigPtrB(NULL,"sigPtrB");

  sig.setConstant( 1.56 );

  SignalBase<int> & sigRef = sig;
  SignalBase<int> & sigPtrARef = sigPtrA, & sigPtrBRef = sigPtrB;
  sigPtrARef.plug( & sigRef );
  sigPtrBRef.plug( & sigPtrARef );

  cout << "Sig = ";  sigRef.get(cout); cout << std::endl;
  cout << "SigPtrA = ";  sigPtrARef.get(cout); cout << std::endl;
  cout << "SigPtrB = ";  sigPtrBRef.get(cout); cout << std::endl;


  return 0;
}
