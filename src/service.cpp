
#include "service.hpp"

#define PHP_ASIO_RUN_LOOP(meth) \
    zval* error = nullptr; \
    ZEND_PARSE_PARAMETERS_START(0, 1) \
        Z_PARAM_OPTIONAL \
        Z_PARAM_ZVAL(error) \
    ZEND_PARSE_PARAMETERS_END(); \
    boost::system::error_code ec; \
    auto handler_count = meth(ec); \
    if (error) { \
        ZVAL_DEREF(error); \
        ZVAL_LONG(error, static_cast<zend_long>(ec.value())); \
    } \
    RETVAL_LONG(static_cast<zend_long>(handler_count))

#define PHP_ASIO_NOTIFY_FORK(ev) \
    try { \
        io_service_.notify_fork(boost::asio::io_service::ev); \
    } catch (const boost::system::system_error& err) { \
        RETURN_LONG(static_cast<zend_long>(err.code().value())); \
    } \
    RETVAL_LONG(0)

namespace Asio
{
    P3_METHOD(Service, addTimer)
    {
        PHP_ASIO_OBJ_ALLOC(timer, Timer, io_service_);
        RETVAL_OBJ(timer);
    }

    P3_METHOD(Service, addSignal)
    {
        PHP_ASIO_OBJ_ALLOC(signal, Signal, io_service_);
        RETVAL_OBJ(signal);
    }

    P3_METHOD(Service, addTcpResolver)
    {
        PHP_ASIO_OBJ_ALLOC(resolver, TcpResolver, io_service_);
        RETVAL_OBJ(resolver);
    }

    P3_METHOD(Service, addUdpResolver)
    {
        PHP_ASIO_OBJ_ALLOC(resolver, UdpResolver, io_service_);
        RETVAL_OBJ(resolver);
    }

    P3_METHOD(Service, addTcpSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, TcpSocket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(Service, addUdpSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, UdpSocket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(Service, addUnixSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, UnixSocket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(Service, addUdgSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, UdgSocket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(Service, addTcpAcceptor)
    {
        PHP_ASIO_OBJ_ALLOC(acceptor, TcpAcceptor, io_service_);
        RETVAL_OBJ(acceptor);
    }

    P3_METHOD(Service, addUnixAcceptor)
    {
        PHP_ASIO_OBJ_ALLOC(acceptor, UnixAcceptor, io_service_);
        RETVAL_OBJ(acceptor);
    }

#ifdef ENABLE_STRAND
    P3_METHOD(Service, addStrand)
    {
        PHP_ASIO_OBJ_ALLOC(strand, Strand, io_service_);
        RETVAL_OBJ(strand);
    }
#endif // ENABLE_STRAND

    P3_METHOD(Service, run)
    {
        PHP_ASIO_RUN_LOOP(io_service_.run);
    }

    P3_METHOD(Service, runOne)
    {
        PHP_ASIO_RUN_LOOP(io_service_.run_one);
    }

    P3_METHOD(Service, poll)
    {
        PHP_ASIO_RUN_LOOP(io_service_.poll);
    }

    P3_METHOD(Service, pollOne)
    {
        PHP_ASIO_RUN_LOOP(io_service_.poll_one);
    }

    P3_METHOD(Service, stop)
    {
        io_service_.stop();
    }

    P3_METHOD(Service, reset)
    {
        io_service_.reset();
    }

    P3_METHOD(Service, stopped) const
    {
        RETVAL_BOOL(io_service_.stopped());
    }

    P3_METHOD(Service, post)
    {
        PHP_ASIO_DISPATCH_CALLBACK(io_service_.post);
    }

    P3_METHOD(Service, dispatch)
    {
        PHP_ASIO_DISPATCH_CALLBACK(io_service_.dispatch);
    }

    P3_METHOD(Service, forkPrepare)
    {
        PHP_ASIO_NOTIFY_FORK(fork_prepare);
    }

    P3_METHOD(Service, forkParent)
    {
        PHP_ASIO_NOTIFY_FORK(fork_parent);
    }

    P3_METHOD(Service, forkChild)
    {
        PHP_ASIO_NOTIFY_FORK(fork_child);
    }

    PHP_ASIO_CE_DEFINE(Service);
}