//
// Copyright 2010 CNRS
//
// Author: Rohan Budhiraja
//
// This file is part of dynamic-graph.
// dynamic-graph is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
// dynamic-graph is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.  You should
// have received a copy of the GNU Lesser General Public License along
// with dynamic-graph.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_EIGEN_IO_H
#define DYNAMIC_GRAPH_EIGEN_IO_H

#include <iostream>
#include <Eigen/Dense>

#include <boost/format.hpp>

#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/linear-algebra.h>

using dynamicgraph::ExceptionSignal;

  /* \brief Eigen Vector input from istream
   *
   * Input Vector format: [val1 val2 val3 ... valN]
   * e.g. 1 23 32.2 12.12 32
   */

std::istringstream& operator >> (std::istringstream &iss, 
				 dynamicgraph::Vector &inst)
{
  std::vector<double> _stdvec;
  double _dbl_val;
  
  boost::format fmt ("Failed to enter %s as vector. Reenter as [val1 val2 ... valN]");
  fmt %iss.str();
  
  while(iss >> _dbl_val && !iss.fail()) {
    _stdvec.push_back(_dbl_val);
  }
  inst = Eigen::VectorXd::Map(_stdvec.data(), _stdvec.size());
  return iss;
}
//Should not reach here.

/*
  std::istringstream& operator >> (std::istringstream &iss, 
  dynamicgraph::Vector &inst)
    {
      std::vector<double> _stdvec;
      char _ch;
      double _dbl_val;

      boost::format fmt ("Failed to enter %s as vector. Reenter as [val1 val2 ... valN]");
      fmt %iss.str();

      if(iss>> _ch && _ch != '['){
	throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
      }
      else{
	while(iss >> _dbl_val && !iss.fail()){
	  _stdvec.push_back(_dbl_val);
	}
	if(iss>> _ch && _ch != ']'){
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	}
	else {
	  inst = Eigen::VectorXd::Map(_stdvec.data(), _stdvec.size());
	  return iss;
	}
	//Should not reach here.
      }
    }
 */


  /* \brief Eigen Matrix input from istream
   *
   * Matrix format: [[val11 val12 val13 ... val1N] ... [valM1 valM2 ... valMN]]
   * e.g. [[1 23 32.2 12.12 32][2 32 23 92.01 19.2]]
   */

  std::istringstream& operator >> (std::istringstream &iss, 
				   dynamicgraph::Matrix &inst)
    {
      std::vector<dynamicgraph::Vector> _stdmat;
      char _ch;
      int _vec_size;
      bool _vec_size_set = false;

      boost::format fmt 
	("Failed to enter %s as matrix. Reenter as [[val11 val12 ... val1N]...[valM1 valM2 ... valMN]]. Check that vector sizes are consistent.");
      fmt %iss.str();

      if(iss>> _ch && _ch != '['){
	throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
      }
      else{
	dynamicgraph::Vector _eigvec;
	while(iss >> _eigvec && !iss.fail()){
	  if (!_vec_size_set) {
	    _vec_size = _eigvec.size();
	    _vec_size_set = true;
	  }
	  else {
	    if (_eigvec.size() != _vec_size) {
	      throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	    }
	  }
	  _stdmat.push_back(_eigvec);
	}
	if(iss>> _ch && _ch != ']'){
	  throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
	}
	else {
	  inst.resize(_stdmat.size(),_vec_size);
	  for (unsigned int i =0;i<_stdmat.size(); i++) {
	    inst.row(i) = _stdmat[i];
	  }
	}
      }
      return iss;
    }
//}


#endif //DYNAMIC_GRAPH_EIGEN_IO_H


