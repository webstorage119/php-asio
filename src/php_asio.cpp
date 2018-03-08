/**
 * php-asio/php_asio.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include <php.h>
#include <ext/standard/info.h>

#include "php_asio.hpp"
#include "p3.hpp"
#include "service.hpp"
#include "wrapped_handler.hpp"

/* {{{ arg_info */

ZEND_BEGIN_ARG_INFO(service_run_arginfo, 0)
    ZEND_ARG_INFO(1, ec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(service_run_one_arginfo, 0)
    ZEND_ARG_INFO(1, ec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(service_poll_arginfo, 0)
    ZEND_ARG_INFO(1, ec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(service_poll_one_arginfo, 0)
    ZEND_ARG_INFO(1, ec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(service_dispatch_arginfo, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

#ifdef ENABLE_STRAND
ZEND_BEGIN_ARG_INFO(strand_dispatch_arginfo, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()
#endif // ENABLE_STRAND

ZEND_BEGIN_ARG_INFO(strand_wrap_arginfo, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(timer_expire_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, time, IS_LONG, 0)
    ZEND_ARG_INFO(0, use_timestamp)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(timer_wait_arginfo, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(signal_add_arginfo, 0)
    ZEND_ARG_VARIADIC_INFO(0, sig_num)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(signal_remove_arginfo, 0)
    ZEND_ARG_VARIADIC_INFO(0, sig_num)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(signal_wait_arginfo, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(resolver_resolve_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, host, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, service, IS_STRING, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(socket_available_arginfo, 0)
    ZEND_ARG_INFO(1, ec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(socket_at_mark_arginfo, 0)
    ZEND_ARG_INFO(1, ec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(inet_socket_open_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, inet6, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(inet_socket_bind_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, address, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, port, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(inet_socket_assign_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, inet6, _IS_BOOL, 0)
    ZEND_ARG_INFO(0, native_handle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(local_socket_assign_arginfo, 0)
    ZEND_ARG_INFO(0, native_handle)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(stream_socket_read_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, read_some, _IS_BOOL, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(stream_socket_write_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, write_some, _IS_BOOL, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(local_socket_bind_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(datagram_socket_recv_from_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(tcp_socket_connect_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, address, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, port, IS_LONG, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(unix_socket_connect_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(udp_socket_send_to_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, address, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, port, IS_LONG, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(udg_socket_send_to_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(acceptor_listen_arginfo, 0)
    ZEND_ARG_TYPE_INFO(0, backlog, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(acceptor_accept_arginfo, 0)
    ZEND_ARG_CALLABLE_INFO(0, callback, 0)
    ZEND_ARG_INFO(0, argument)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(stream_descriptor_assign_arginfo, 0)
    ZEND_ARG_INFO(0, native_handle)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ function_entry */

static zend_function_entry service_methods[] = {
    P3_ME_D(asio::service, addTimer, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addSignal, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addTcpResolver, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addUdpResolver, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addTcpSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addUdpSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addUnixSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addUdgSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addTcpAcceptor, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addUnixAcceptor, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, addStreamDescriptor, nullptr, ZEND_ACC_PUBLIC)
#ifdef ENABLE_STRAND
    P3_ME_D(asio::service, addStrand, nullptr, ZEND_ACC_PUBLIC)
#endif // ENABLE_STRAND
    P3_ME_D(asio::service, run, service_run_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, runOne, service_run_one_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, poll, service_poll_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, pollOne, service_poll_one_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, stop, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, reset, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, stopped, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, post, service_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, dispatch, service_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, forkPrepare, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, forkParent, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::service, forkChild, nullptr, ZEND_ACC_PUBLIC)
#ifdef ENABLE_COROUTINE
    P3_STATIC_ME(asio::future, lastError, nullptr, ZEND_ACC_PUBLIC)
#endif // ENABLE_COROUTINE
    PHP_FE_END
};

#ifdef ENABLE_STRAND
static zend_function_entry strand_methods[] = {
    P3_ME_D(asio::strand, dispatch, strand_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::strand, post, strand_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::strand, runningInThisThread, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::strand, wrap, strand_wrap_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::strand, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry wrapped_handler_methods[] = {
    P3_ME_D(asio::wrapped_handler, __invoke, nullptr, ZEND_ACC_PUBLIC | ZEND_ACC_CALL_VIA_TRAMPOLINE)
    PHP_FE_END
};
#endif // ENABLE_STRAND

static zend_function_entry io_object_method[] = {
    P3_ABSTRACT_ME(cancel, nullptr)
    P3_ABSTRACT_ME(destroy, nullptr)
    PHP_FE_END
};

static zend_function_entry timer_methods[] = {
    P3_ME_D(asio::timer, expire, timer_expire_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::timer, wait, timer_wait_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::timer, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::timer, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry signal_methods[] = {
    P3_ME_D(asio::signal, add, signal_add_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::signal, remove, signal_remove_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::signal, wait, signal_wait_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::signal, clear, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::signal, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::signal, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry resolver_methods[] = {
    P3_ABSTRACT_ME(resolve, resolver_resolve_arginfo)
    P3_ABSTRACT_ME(cancel, nullptr)
    PHP_FE_END
};

static zend_function_entry tcp_resolver_methods[] = {
    P3_ME_D(asio::tcp_resolver, resolve, resolver_resolve_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_resolver, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_resolver, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry udp_resolver_methods[] = {
    P3_ME_D(asio::udp_resolver, resolve, resolver_resolve_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_resolver, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_resolver, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry socket_methods[] = {
    P3_ABSTRACT_ME(available, socket_available_arginfo)
    P3_ABSTRACT_ME(atMark, socket_at_mark_arginfo)
    P3_ABSTRACT_ME(close, nullptr)
    PHP_FE_END
};

static zend_function_entry inet_socket_methods[] = {
    P3_ABSTRACT_ME(open, inet_socket_open_arginfo)
    P3_ABSTRACT_ME(assign, inet_socket_assign_arginfo)
    P3_ABSTRACT_ME(bind, inet_socket_bind_arginfo)
    P3_ABSTRACT_ME(remoteAddr, nullptr)
    P3_ABSTRACT_ME(remotePort, nullptr)
    PHP_FE_END
};

static zend_function_entry local_socket_methods[] = {
    P3_ABSTRACT_ME(open, nullptr)
    P3_ABSTRACT_ME(assign, local_socket_assign_arginfo)
    P3_ABSTRACT_ME(bind, local_socket_bind_arginfo)
    P3_ABSTRACT_ME(remotePath, nullptr)
    PHP_FE_END
};

static zend_function_entry stream_socket_methods[] = {
    P3_ABSTRACT_ME(read, stream_socket_read_arginfo)
    P3_ABSTRACT_ME(write, stream_socket_write_arginfo)
    PHP_FE_END
};

static zend_function_entry datagram_socket_methods[] = {
    P3_ABSTRACT_ME(recvFrom, datagram_socket_recv_from_arginfo)
    PHP_FE_END
};

static zend_function_entry tcp_socket_methods[] = {
    P3_ME(asio::tcp_socket, open, open_inet, inet_socket_open_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(asio::tcp_socket, assign, assign_inet, inet_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(asio::tcp_socket, bind, bind_inet, inet_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, connect, tcp_socket_connect_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, read, stream_socket_read_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, write, stream_socket_write_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, remoteAddr, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, remotePort, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, close, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_socket, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry udp_socket_methods[] = {
    P3_ME(asio::udp_socket, open, open_inet, inet_socket_open_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(asio::udp_socket, assign, assign_inet, inet_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(asio::udp_socket, bind, bind_inet, inet_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, sendTo, udp_socket_send_to_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, recvFrom, datagram_socket_recv_from_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, remoteAddr, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, remotePort, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, close, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udp_socket, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry unix_socket_methods[] = {
    P3_ME(asio::unix_socket, open, open_local, nullptr, ZEND_ACC_PUBLIC)
    P3_ME(asio::unix_socket, assign, assign_local, local_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(asio::unix_socket, bind, bind_local, local_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, connect, unix_socket_connect_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, read, stream_socket_read_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, write, stream_socket_write_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, remotePath, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, close, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_socket, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry udg_socket_methods[] = {
    P3_ME(asio::udg_socket, open, open_local, nullptr, ZEND_ACC_PUBLIC)
    P3_ME(asio::udg_socket, assign, assign_local, local_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(asio::udg_socket, bind, bind_local, local_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, sendTo, udg_socket_send_to_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, recvFrom, datagram_socket_recv_from_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, remotePath, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, close, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::udg_socket, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry acceptor_methods[] = {
    P3_ABSTRACT_ME(listen, acceptor_listen_arginfo)
    P3_ABSTRACT_ME(accept, acceptor_accept_arginfo)
    P3_ABSTRACT_ME(close, nullptr)
    PHP_FE_END
};

static zend_function_entry tcp_acceptor_methods[] = {
    P3_ME_D(asio::tcp_acceptor, open, inet_socket_open_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_acceptor, assign, inet_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_acceptor, bind, inet_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_acceptor, listen, acceptor_listen_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_acceptor, accept, acceptor_accept_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_acceptor, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_acceptor, close, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::tcp_acceptor, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry unix_acceptor_methods[] = {
    P3_ME_D(asio::unix_acceptor, open, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_acceptor, assign, local_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_acceptor, bind, local_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_acceptor, listen, acceptor_listen_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_acceptor, accept, acceptor_accept_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_acceptor, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_acceptor, close, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::unix_acceptor, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry stream_descriptor_methods[] = {
    P3_ME_D(asio::stream_descriptor, assign, stream_descriptor_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::stream_descriptor, isOpen, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::stream_descriptor, read, stream_socket_read_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::stream_descriptor, write, stream_socket_write_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::stream_descriptor, release, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::stream_descriptor, cancel, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(asio::stream_descriptor, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* }}} */

/* {{{ interfaces class_entry */

auto io_object_ce = p3::interface_init("Asio\\IoObject", io_object_method);
auto resolver_ce = p3::interface_init("Asio\\Resolver", resolver_methods);
auto socket_ce = p3::interface_init("Asio\\Socket", socket_methods);
auto inet_socket_ce = p3::interface_init("Asio\\InetSocket", inet_socket_methods);
auto local_socket_ce = p3::interface_init("Asio\\LocalSocket", local_socket_methods);
auto stream_socket_ce = p3::interface_init("Asio\\StreamSocket", stream_socket_methods);
auto datagram_socket_ce = p3::interface_init("Asio\\DatagramSocket", datagram_socket_methods);
auto acceptor_ce = p3::interface_init("Asio\\Acceptor", acceptor_methods);

/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(asio)
{
    socket_ce->parent = io_object_ce;
    resolver_ce->parent = io_object_ce;
    acceptor_ce->parent = io_object_ce;
    inet_socket_ce->parent = socket_ce;
    local_socket_ce->parent = socket_ce;
    stream_socket_ce->parent = socket_ce;
    datagram_socket_ce->parent = socket_ce;
    p3::class_init<asio::service>("Asio\\Service", service_methods);
    auto ce = p3::class_init<asio::timer>("Asio\\Timer", timer_methods);
    zend_class_implements(ce, 1, io_object_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::signal>("Asio\\Signal", signal_methods);
    zend_class_implements(ce, 1, io_object_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::tcp_resolver>("Asio\\TcpResolver", tcp_resolver_methods);
    zend_class_implements(ce, 1, resolver_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::udp_resolver>("Asio\\UdpResolver", udp_resolver_methods);
    zend_class_implements(ce, 1, resolver_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::tcp_socket>("Asio\\TcpSocket", tcp_socket_methods);
    zend_class_implements(ce, 2, inet_socket_ce, stream_socket_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::udp_socket>("Asio\\UdpSocket", udp_socket_methods);
    zend_class_implements(ce, 2, inet_socket_ce, datagram_socket_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::unix_socket>("Asio\\UnixSocket", unix_socket_methods);
    zend_class_implements(ce, 2, local_socket_ce, stream_socket_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::udg_socket>("Asio\\UdgSocket", udg_socket_methods);
    zend_class_implements(ce, 2, local_socket_ce, datagram_socket_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::tcp_acceptor>("Asio\\TcpAcceptor", tcp_acceptor_methods);
    zend_class_implements(ce, 1, acceptor_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::unix_acceptor>("Asio\\UnixAcceptor", unix_acceptor_methods);
    zend_class_implements(ce, 1, acceptor_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::stream_descriptor>("Asio\\StreamDescriptor", stream_descriptor_methods);
    zend_class_implements(ce, 1, io_object_ce);
    ce->ce_flags |= ZEND_ACC_FINAL;
#ifdef ENABLE_COROUTINE
    ce = p3::class_init<asio::future>("Asio\\Future", nullptr);
    ce->ce_flags |= ZEND_ACC_FINAL;
#endif // ENABLE_COROUTINE
#ifdef ENABLE_STRAND
    ce = p3::class_init<asio::strand>("Asio\\Strand", strand_methods);
    ce->ce_flags |= ZEND_ACC_FINAL;
    ce = p3::class_init<asio::wrapped_handler>("Asio\\WrappedHandler", wrapped_handler_methods);
    ce->ce_flags |= ZEND_ACC_FINAL;
#endif // ENABLE_STRAND
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(asio)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Coroutine support",
#ifdef ENABLE_COROUTINE
        "enabled"
#else
        "disabled"
#endif // ENABLE_COROUTINE
    );
    php_info_print_table_header(2, "Strand support",
#ifdef ENABLE_STRAND
        "enabled"
#else
        "disabled"
#endif // ENABLE_STRAND
    );
    php_info_print_table_header(2, "Null buffers support",
#ifdef ENABLE_NULL_BUFFERS
        "enabled"
#else
        "disabled"
#endif // ENABLE_NULL_BUFFERS
    );
	php_info_print_table_end();
}
/* }}} */

/* {{{ asio_module_entry */
zend_module_entry asio_module_entry = {
	STANDARD_MODULE_HEADER,
	"asio",
	nullptr,              
	PHP_MINIT(asio),
	nullptr,
	nullptr,
	nullptr,
	PHP_MINFO(asio),
	PHP_ASIO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ASIO
ZEND_GET_MODULE(asio)
#endif // COMPILE_DL_ASIO
