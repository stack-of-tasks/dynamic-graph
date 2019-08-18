/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 */

#include <dynamic-graph/exception-traces.h>
#include <stdarg.h>
#include <cstdio>

using namespace dynamicgraph;
/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

const std::string ExceptionTraces::EXCEPTION_NAME = "Traces";

ExceptionTraces::ExceptionTraces
(const ExceptionTraces::ErrorCodeEnum& errcode,
 const std::string& msg)
    : ExceptionAbstract(errcode, msg) {}

ExceptionTraces::ExceptionTraces
(const ExceptionTraces::ErrorCodeEnum& errcode,
 const std::string& msg,
 const char* format, ...)
    : ExceptionAbstract(errcode, msg)
{
  va_list args;
  va_start(args, format);

  const unsigned int SIZE = 256;
  char buffer[SIZE];
  vsnprintf(buffer, SIZE, format, args);

  message += buffer;

  va_end(args);
}

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
