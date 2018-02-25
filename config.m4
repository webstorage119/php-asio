PHP_ARG_ENABLE(asio,               for asio support,
[  --enable-asio               Enable asio support        ])

PHP_ARG_ENABLE(asio-coroutine,     for coroutine support,
[  --disable-asio-coroutine    Disable coroutine support  ], yes, no)

PHP_ARG_ENABLE(asio-strand,        for strand support,
[  --enable-asio-strand        Enable strand support      ], no, no)

PHP_ARG_ENABLE(asio-null-buffers,  for null buffer support,
[  --enable-asio-null-buffers  Enable null buffers        ], no, no)

PHP_ARG_ENABLE(asio-debug,         for debug support,
[  --enable-asio-debug         Compile with debug symbols ], no, no)

if test "$PHP_ASIO" != "no"; then
  PHP_REQUIRE_CXX()

  CXXFLAGS="-std=c++14"
  if test "$PHP_ASIO_DEBUG" != "no"; then
    CXXFLAGS+=" -g -O0"
  else
    CXXFLAGS+=" -O2"
  fi

  PHP_ASIO_SRC="src/php_asio.cpp \
    src/service.cpp \
    src/wrapped_handler.cpp \
    src/future.cpp \
    src/strand.cpp \
    src/timer.cpp \
    src/signal.cpp \
    src/resolver.cpp \
    src/socket.cpp \
    src/acceptor.cpp \
    src/stream_descriptor.cpp"
  
  PHP_NEW_EXTENSION(asio, $PHP_ASIO_SRC, $ext_shared, cli,, yes)

  if test "$PHP_ASIO_COROUTINE" != "no"; then
    AC_DEFINE(ENABLE_COROUTINE, 1, [ ])
  fi
  if test "$PHP_ASIO_STRAND" != "no"; then
    AC_DEFINE(ENABLE_STRAND, 1, [ ])
  fi
  if test "$PHP_ASIO_NULL_BUFFERS" != "no"; then
    AC_DEFINE(ENABLE_NULL_BUFFERS, 1, [ ])
  fi

  PHP_ADD_LIBRARY(boost_system, 1, ASIO_SHARED_LIBADD)
  PHP_ADD_LIBRARY(boost_filesystem, 1, ASIO_SHARED_LIBADD)
  PHP_SUBST(ASIO_SHARED_LIBADD)
fi
