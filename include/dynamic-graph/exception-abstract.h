// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_EXCEPTION_ABSTRACT_H
#define DYNAMIC_GRAPH_EXCEPTION_ABSTRACT_H
#include <string>

#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/fwd.hh>

// Uncomment this macros to have lines parameter on the throw display
// #define DYNAMIC-GRAPH_EXCEPTION_PASSING_PARAM

#define DG_RETHROW                                                             \
  (const ::dynamicgraph::ExceptionAbstract &err) { throw err; }

#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM
#define DG_THROW                                                               \
  throw ::dynamicgraph::ExceptionAbstract::Param(__LINE__, __FUNCTION__,       \
                                                 __FILE__) +
#else
#define DG_THROW throw
#endif // DYNAMICGRAPH_EXCEPTION_PASSING_PARAM

namespace dynamicgraph {
/// \ingroup error
///
/// \brief Abstract root class for all dynamic-graph exceptions.
class DYNAMIC_GRAPH_DLLAPI ExceptionAbstract : public std::exception {
public:
  /// \ingroup error
  ///
  /// \brief Class owned by exceptions to store error locations.
  class Param {
  public:
    static const int BUFFER_SIZE = 80;

    Param(const int &_line, const char *_function, const char *_file);
    Param()
        : functionPTR(), function(), line(), filePTR(), file(),
          pointersSet(false), set(false) {}
    Param &initCopy(const Param &p);

    const char *functionPTR;
    char function[BUFFER_SIZE];
    int line;
    const char *filePTR;
    char file[BUFFER_SIZE];
    bool pointersSet;
    bool set;
  };

  /// \brief Categories error code.
  ///
  /// Each value matches categories used by a subclass of
  /// ExceptionAbstract.
  ///
  /// This is the programmer responsibility to make sure there is
  /// enough room between two categories error code.
  enum ExceptionEnum {
    ABSTRACT = 0,
    SIGNAL = 100,
    FACTORY = 200,
    TRACES = 300,
    TOOLS = 700
  };

  static const std::string EXCEPTION_NAME;

  explicit ExceptionAbstract(const int &code, const std::string &msg = "");
  virtual ~ExceptionAbstract() throw() {}

  virtual const std::string &getExceptionName() const { return EXCEPTION_NAME; }

  /// \brief Access to the error code.
  int getCode() const;

  /// \brief Reference access to the error message (can be empty).
  const std::string &getStringMessage() const;

  /// \brief Access to the pointer on the array of \e char related
  /// to the error string.
  ///
  /// Cannot be \e NULL.
  const char *getMessage() const;

  virtual const char *what() const throw() {
    return getStringMessage().c_str();
  }

  /// \brief Print the error structure.
  DYNAMIC_GRAPH_DLLAPI friend std::ostream &
  operator<<(std::ostream &os, const ExceptionAbstract &err);

protected:
  /// \brief Error code.
  /// \sa ErrorCodeEnum
  int code;

  /// \brief Error message (can be empty).
  std::string message;

#ifdef DYNAMICGRAPH_EXCEPTION_PASSING_PARAM
  /// \brief Optional mutable attribute to store exception location.
  ///
  /// Only present if DYNAMICGRAPH_EXCEPTION_PASSING_PARAM
  /// preprocessor symbol exists.
  mutable Param p;

  template <class Exc>
  friend const Exc &operator+(const ExceptionAbstract::Param &p, const Exc &e) {
    e.p.initCopy(p);
    return e;
  }

  template <class Exc>
  friend Exc &operator+(const ExceptionAbstract::Param &p, Exc &e) {
    e.p.initCopy(p);
    return e;
  }

#endif // DYNAMICGRAPH_EXCEPTION_PASSING_PARAM

private:
  /// \brief Forbid the empty constructor (private).
  ExceptionAbstract();
};
} // end of namespace dynamicgraph

#endif //! DYNAMIC_GRAPH_EXCEPTION_ABSTRACT_H
