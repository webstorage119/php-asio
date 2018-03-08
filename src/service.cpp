/**
 * php-asio/service.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

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

namespace asio
{
    P3_METHOD(service, addTimer)
    {
        PHP_ASIO_OBJ_ALLOC(timer, asio::timer, io_service_);
        RETVAL_OBJ(timer);
    }

    P3_METHOD(service, addSignal)
    {
        PHP_ASIO_OBJ_ALLOC(signal, asio::signal, io_service_);
        RETVAL_OBJ(signal);
    }

    P3_METHOD(service, addTcpResolver)
    {
        PHP_ASIO_OBJ_ALLOC(resolver, tcp_resolver, io_service_);
        RETVAL_OBJ(resolver);
    }

    P3_METHOD(service, addUdpResolver)
    {
        PHP_ASIO_OBJ_ALLOC(resolver, udp_resolver, io_service_);
        RETVAL_OBJ(resolver);
    }

    P3_METHOD(service, addTcpSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, tcp_socket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(service, addUdpSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, udp_socket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(service, addUnixSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, unix_socket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(service, addUdgSocket)
    {
        PHP_ASIO_OBJ_ALLOC(socket, udg_socket, io_service_);
        RETVAL_OBJ(socket);
    }

    P3_METHOD(service, addTcpAcceptor)
    {
        PHP_ASIO_OBJ_ALLOC(acceptor, tcp_acceptor, io_service_);
        RETVAL_OBJ(acceptor);
    }

    P3_METHOD(service, addUnixAcceptor)
    {
        PHP_ASIO_OBJ_ALLOC(acceptor, unix_acceptor, io_service_);
        RETVAL_OBJ(acceptor);
    }

    P3_METHOD(service, addStreamDescriptor)
    {
        PHP_ASIO_OBJ_ALLOC(descriptor, stream_descriptor, io_service_);
        RETVAL_OBJ(descriptor);
    }

#ifdef ENABLE_STRAND
    P3_METHOD(service, addStrand)
    {
        PHP_ASIO_OBJ_ALLOC(strand, asio::strand, io_service_);
        RETVAL_OBJ(strand);
    }
#endif // ENABLE_STRAND

    P3_METHOD(service, run)
    {
        PHP_ASIO_RUN_LOOP(io_service_.run);
    }

    P3_METHOD(service, runOne)
    {
        PHP_ASIO_RUN_LOOP(io_service_.run_one);
    }

    P3_METHOD(service, poll)
    {
        PHP_ASIO_RUN_LOOP(io_service_.poll);
    }

    P3_METHOD(service, pollOne)
    {
        PHP_ASIO_RUN_LOOP(io_service_.poll_one);
    }

    P3_METHOD(service, stop)
    {
        io_service_.stop();
    }

    P3_METHOD(service, reset)
    {
        io_service_.reset();
    }

    P3_METHOD(service, stopped) const
    {
        RETVAL_BOOL(io_service_.stopped());
    }

    P3_METHOD(service, post)
    {
        PHP_ASIO_DISPATCH_CALLBACK(io_service_.post);
    }

    P3_METHOD(service, dispatch)
    {
        PHP_ASIO_DISPATCH_CALLBACK(io_service_.dispatch);
    }

    P3_METHOD(service, forkPrepare)
    {
        PHP_ASIO_NOTIFY_FORK(fork_prepare);
    }

    P3_METHOD(service, forkParent)
    {
        PHP_ASIO_NOTIFY_FORK(fork_parent);
    }

    P3_METHOD(service, forkChild)
    {
        PHP_ASIO_NOTIFY_FORK(fork_child);
    }

    PHP_ASIO_CE_DEFINE(service);
}