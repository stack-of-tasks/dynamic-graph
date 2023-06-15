// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_SIGNAL_ARRAY_H
#define DYNAMIC_GRAPH_SIGNAL_ARRAY_H
#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/signal-base.h>
#include <stdio.h>

#include <vector>

namespace dynamicgraph {

/// \ingroup dgraph
///
/// \brief TODO
template <class Time>
class SignalArray_const {
 public:
  static const int DEFAULT_SIZE = 20;

 protected:
  std::vector<const SignalBase<Time> *> const_array;
  unsigned int size, rank;

 public:
  SignalArray_const<Time>(const unsigned int &sizeARG = DEFAULT_SIZE)
      : const_array(sizeARG), size(sizeARG), rank(0) {}

  SignalArray_const<Time>(const SignalBase<Time> &sig)
      : const_array(DEFAULT_SIZE), size(DEFAULT_SIZE), rank(0) {
    addElmt(&sig);
  }

  SignalArray_const<Time>(const SignalArray<Time> &siga)
      : const_array(siga.getSize()),
        size(siga.getSize()),
        rank(siga.getSize()) {
    for (unsigned int i = 0; i < rank; ++i) const_array[i] = &siga[i];
  }

  SignalArray_const<Time>(const SignalArray_const<Time> &siga)
      : const_array(siga.getSize()),
        size(siga.getSize()),
        rank(siga.getSize()) {
    for (unsigned int i = 0; i < rank; ++i) const_array[i] = &siga[i];
  }

  virtual ~SignalArray_const<Time>() {}

 protected:
  void addElmt(const SignalBase<Time> *el) {
    if (rank >= size) {
      size += DEFAULT_SIZE;
      const_array.resize(size);
    }
    const_array[rank++] = el;
  }

 public:
  virtual SignalArray_const<Time> &operator<<(const SignalBase<Time> &sig) {
    addElmt(&sig);
    return *this;
  }

 public:
  virtual const SignalBase<Time> &operator[](const unsigned int &idx) const {
    return *const_array[idx];
  }
  virtual unsigned int getSize() const { return rank; }
};

template <class Time>
SignalArray_const<Time> operator<<(const SignalBase<Time> &sig1,
                                   const SignalBase<Time> &sig2) {
  SignalArray_const<Time> res(sig1);
  res << sig2;
  return res;
}

/// \ingroup dgraph
///
/// \brief TODO
template <class Time>
class SignalArray : public SignalArray_const<Time> {
 public:
  using SignalArray_const<Time>::DEFAULT_SIZE;
  using SignalArray_const<Time>::size;
  using SignalArray_const<Time>::rank;

 protected:
  mutable std::vector<SignalBase<Time> *> array;

 public:
  SignalArray<Time>(const unsigned int &sizeARG = DEFAULT_SIZE)
      : SignalArray_const<Time>(0), array(sizeARG) {
    size = sizeARG;
  }

  SignalArray<Time>(SignalBase<Time> &sig)
      : SignalArray_const<Time>(0), array(DEFAULT_SIZE) {
    size = DEFAULT_SIZE;
    addElmt(&sig);
  }

  SignalArray<Time>(const SignalArray<Time> &siga)
      : SignalArray_const<Time>(siga.getSize()), array(siga.getSize()) {
    rank = siga.getSize();
    for (unsigned int i = 0; i < rank; ++i) array[i] = &siga[i];
  }

  virtual ~SignalArray<Time>() {}

 protected:
  void addElmt(SignalBase<Time> *el) {
    if (rank >= size) {
      size += DEFAULT_SIZE;
      array.resize(size);
    }
    array[rank++] = el;
  }

 public:
  virtual SignalArray<Time> &operator<<(SignalBase<Time> &sig) {
    addElmt(&sig);
    return *this;
  }

  virtual SignalArray_const<Time> operator<<(
      const SignalBase<Time> &sig) const {
    SignalArray_const<Time> res(*this);
    res << sig;
    return res;
  }

  virtual SignalBase<Time> &operator[](const unsigned int &idx) const {
    return *array[idx];
  }
};

template <class Time>
SignalArray<Time> operator<<(SignalBase<Time> &sig1, SignalBase<Time> &sig2) {
  SignalArray<Time> res(sig1);
  res << sig2;
  return res;
}

DYNAMIC_GRAPH_DLLAPI extern SignalArray<sigtime_t> sotNOSIGNAL;

}  // end of namespace dynamicgraph.

#endif  //! DYNAMIC_GRAPH_SIGNAL_ARRAY_H
