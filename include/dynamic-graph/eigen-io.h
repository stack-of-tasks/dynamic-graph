//
// Copyright 2016 CNRS
//
// Author: Rohan Budhiraja
//

#ifndef DYNAMIC_GRAPH_EIGEN_IO_H
#define DYNAMIC_GRAPH_EIGEN_IO_H

#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#pragma GCC diagnostic push
#pragma GCC system_header
#include <Eigen/Geometry>
#pragma GCC diagnostic pop
#include <dynamic-graph/exception-signal.h>
#include <dynamic-graph/linear-algebra.h>

using dynamicgraph::ExceptionSignal;

// TODO: Eigen 3.3 onwards has a global Eigen::Index definition.
// If Eigen version is updated, use Eigen::Index instead of this macro.

/* \brief Eigen Vector input from istream
 *
 * Input Vector format: [N](val1,val2,val3,...,valN)
 * e.g. [5](1,23,32.2,12.12,32)
 */
namespace Eigen {
typedef EIGEN_DEFAULT_DENSE_INDEX_TYPE eigen_index;

inline std::istringstream &operator>>(std::istringstream &iss,
                                      dynamicgraph::Vector &inst) {
  unsigned int _size;
  double _dbl_val;
  char _ch;
  boost::format fmt(
      "Failed to enter %s as vector."
      " Reenter as [N](val1,val2,val3,...,valN)");
  fmt % iss.str();
  if (iss >> _ch && _ch != '[') {
    throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
  } else {
    if (iss >> _size && !iss.fail()) {
      inst.resize(_size);
    } else
      throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
    if (iss >> _ch && _ch != ']')
      throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
    else {
      if (iss >> _ch && _ch != '(')
        throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
      else {
        for (unsigned int i = 0; i < _size; i++) {
          iss >> _dbl_val;
          if (iss.peek() == ',' || iss.peek() == ' ') iss.ignore();
          inst(i) = _dbl_val;
        }
        if (iss >> _ch && _ch != ')')
          throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
      }
    }
  }
  return iss;
}

/* \brief Eigen Matrix input from istream
 *
 * Matrix format: [M,N]((val11,val12,val13,...,val1N),...,
 * (valM1,valM2,...,valMN))
 * e.g. [2,5]((1 23 32.2 12.12 32),(2 32 23 92.01 19.2))
 */

template <typename Derived>
inline std::istringstream &operator>>(std::istringstream &iss,
                                      DenseBase<Derived> &inst) {
  unsigned int _colsize;
  unsigned int _rowsize;
  double _dbl_val;
  char _ch;
  boost::format fmt(
      "Failed to enter %s as matrix. Reenter as "
      "((val11,val12,val13,...,val1N),"
      "...,(valM1,valM2,...,valMN))");
  MatrixXd _tmp_matrix;
  fmt % iss.str();
  if (iss >> _ch && _ch != '[') {
    throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
  } else {
    iss >> _rowsize;
    if (iss.peek() == ',' || iss.peek() == ' ') iss.ignore();
    iss >> _colsize;
    if (iss.fail())
      throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
    else {
      _tmp_matrix.resize(_rowsize, _colsize);
      if (iss >> _ch && _ch != ']')
        throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
      else {
        if (iss >> _ch && _ch != '(')
          throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
        else {
          for (unsigned int j = 0; j < _rowsize; j++) {
            if (iss >> _ch && _ch != '(')
              throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
            for (unsigned int i = 0; i < _colsize; i++) {
              iss >> _dbl_val;
              if (iss.peek() == ',' || iss.peek() == ' ') iss.ignore();
              _tmp_matrix(j, i) = _dbl_val;
            }
            if (iss >> _ch && _ch != ')')
              throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
            if (iss.peek() == ',' || iss.peek() == ' ') iss.ignore();
          }
          if (iss >> _ch && _ch != ')')
            throw ExceptionSignal(ExceptionSignal::GENERIC, fmt.str());
        }
      }
    }
  }
  inst = _tmp_matrix;
  return iss;
}

inline std::istringstream &operator>>(std::istringstream &iss,
                                      Transform<double, 3, Affine> &inst) {
  MatrixXd M;
  iss >> M;
  inst.matrix() = M;
  return iss;
}

/* \brief Eigen Homogeneous Matrix output
 *
 * Matrix format: [M,N]((val11,val12,val13,...,val1N),...,
 * (valM1,valM2,...,valMN))
 * e.g. [2,5]((1 23 32.2 12.12 32),(2 32 23 92.01 19.2))
 */

inline std::ostream &operator<<(std::ostream &os,
                                Transform<double, 3, Affine> MH) {
  IOFormat boostFmt(StreamPrecision, DontAlignCols, ",", ",", "(", ")", "(",
                    ")");

  os << "[4,4]" << MH.matrix().format(boostFmt);
  return os;
}

inline std::ostream &operator<<(std::ostream &os, AngleAxisd quat) {
  VectorXd v(4);
  v(0) = quat.angle();
  v.tail<3>() = quat.axis();
  os << v;
  return os;
}

inline std::istringstream &operator>>(std::istringstream &iss,
                                      AngleAxisd &inst) {
  VectorXd v(4);
  iss >> v;
  inst.angle() = v(0);
  inst.axis() = v.tail<3>();
  return iss;
}

}  // namespace Eigen

#endif  // DYNAMIC_GRAPH_EIGEN_IO_H
