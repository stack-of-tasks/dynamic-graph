SET(${PROJECT_NAME}_HEADERS
  fwd.hh
  null-ptr.hh
  debug.h
  real-time-logger.h

  dynamic-graph-api.h

  entity.h
  factory.h
  pool.h

  exception-abstract.h
  exception-factory.h
  exception-signal.h
  exception-traces.h

  signal.h
  signal-array.h
  signal-base.h
  signal-ptr.h
  signal-time-dependent.h
  signal-ptr.t.cpp
  signal.t.cpp
  time-dependency.h
  time-dependency.t.cpp
  signal-caster.h
  signal-cast-helper.h
  all-signals.h
  signal-helper.h
  entity-helper.h

  tracer.h
  tracer-real-time.h

  command.h
  eigen-io.h
  linear-algebra.h
  value.h

  command-setter.h
  command-setter.t.cpp
  command-getter.h
  command-getter.t.cpp
  command-direct-getter.h
  command-direct-setter.h
  command-bind.h
  all-commands.h

  logger.h
    )

  INSTALL(FILES ${${PROJECT_NAME}_HEADERS}
    DESTINATION include/${PROJECT_NAME}
  )
