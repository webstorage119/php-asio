
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

/* }}} */

/* {{{ function_entry */

static zend_function_entry service_methods[] = {
    P3_ME_D(Asio::Service, addTimer, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addSignal, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addTcpResolver, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addUdpResolver, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addTcpSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addUdpSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addUnixSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addUdgSocket, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addTcpAcceptor, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, addUnixAcceptor, nullptr, ZEND_ACC_PUBLIC)
#ifdef ENABLE_STRAND
    P3_ME_D(Asio::Service, addStrand, nullptr, ZEND_ACC_PUBLIC)
#endif // ENABLE_STRAND
    P3_ME_D(Asio::Service, run, service_run_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, runOne, service_run_one_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, poll, service_poll_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, pollOne, service_poll_one_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, stop, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, reset, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, stopped, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, post, service_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, dispatch, service_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, forkPrepare, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, forkParent, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Service, forkChild, nullptr, ZEND_ACC_PUBLIC)
#ifdef ENABLE_COROUTINE
    P3_STATIC_ME(Asio::Future, lastError, nullptr, ZEND_ACC_PUBLIC)
#endif // ENABLE_COROUTINE
    PHP_FE_END
};

#ifdef ENABLE_STRAND

static zend_function_entry strand_methods[] = {
    P3_ME_D(Asio::Strand, dispatch, strand_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Strand, post, strand_dispatch_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Strand, runningInThisThread, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Strand, wrap, strand_wrap_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Strand, destroy, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry wrapped_handler_methods[] = {
    P3_ME_D(Asio::WrappedHandler, __invoke, nullptr, ZEND_ACC_PUBLIC | ZEND_ACC_CALL_VIA_TRAMPOLINE)
    PHP_FE_END
};

#endif // ENABLE_STRAND

static zend_function_entry timer_methods[] = {
    P3_ME_D(Asio::Timer, expire, timer_expire_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Timer, wait, timer_wait_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Timer, cancel, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry signal_methods[] = {
    P3_ME_D(Asio::Signal, add, signal_add_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Signal, remove, signal_remove_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Signal, wait, signal_wait_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Signal, clear, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::Signal, cancel, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry resolver_methods[] = {
    P3_ABSTRACT_ME(resolve, resolver_resolve_arginfo)
    P3_ABSTRACT_ME(cancel, nullptr)
    PHP_FE_END
};

static zend_function_entry tcp_resolver_methods[] = {
    P3_ME_D(Asio::TcpResolver, resolve, resolver_resolve_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpResolver, cancel, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry udp_resolver_methods[] = {
    P3_ME_D(Asio::UdpResolver, resolve, resolver_resolve_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpResolver, cancel, nullptr, ZEND_ACC_PUBLIC)
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
    P3_ME(Asio::TcpSocket, open, open_inet, inet_socket_open_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(Asio::TcpSocket, assign, assign_inet, inet_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(Asio::TcpSocket, bind, bind_inet, inet_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, connect, tcp_socket_connect_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, read, stream_socket_read_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, write, stream_socket_write_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, remoteAddr, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, remotePort, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpSocket, close, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry udp_socket_methods[] = {
    P3_ME(Asio::UdpSocket, open, open_inet, inet_socket_open_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(Asio::UdpSocket, assign, assign_inet, inet_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(Asio::UdpSocket, bind, bind_inet, inet_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpSocket, sendTo, udp_socket_send_to_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpSocket, recvFrom, datagram_socket_recv_from_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpSocket, remoteAddr, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpSocket, remotePort, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpSocket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpSocket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdpSocket, close, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry unix_socket_methods[] = {
    P3_ME(Asio::UnixSocket, open, open_local, nullptr, ZEND_ACC_PUBLIC)
    P3_ME(Asio::UnixSocket, assign, assign_local, local_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(Asio::UnixSocket, bind, bind_local, local_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixSocket, connect, unix_socket_connect_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixSocket, read, stream_socket_read_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixSocket, write, stream_socket_write_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixSocket, remotePath, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixSocket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixSocket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixSocket, close, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry udg_socket_methods[] = {
    P3_ME(Asio::UdgSocket, open, open_local, nullptr, ZEND_ACC_PUBLIC)
    P3_ME(Asio::UdgSocket, assign, assign_local, local_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME(Asio::UdgSocket, bind, bind_local, local_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdgSocket, sendTo, udg_socket_send_to_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdgSocket, recvFrom, datagram_socket_recv_from_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdgSocket, remotePath, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdgSocket, available, socket_available_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdgSocket, atMark, socket_at_mark_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UdgSocket, close, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry acceptor_methods[] = {
    P3_ABSTRACT_ME(listen, acceptor_listen_arginfo)
    P3_ABSTRACT_ME(accept, acceptor_accept_arginfo)
    P3_ABSTRACT_ME(close, nullptr)
    PHP_FE_END
};

static zend_function_entry tcp_acceptor_methods[] = {
    P3_ME_D(Asio::TcpAcceptor, open, inet_socket_open_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpAcceptor, assign, inet_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpAcceptor, bind, inet_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpAcceptor, listen, acceptor_listen_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpAcceptor, accept, acceptor_accept_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::TcpAcceptor, close, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static zend_function_entry unix_acceptor_methods[] = {
    P3_ME_D(Asio::UnixAcceptor, open, nullptr, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixAcceptor, assign, local_socket_assign_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixAcceptor, bind, local_socket_bind_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixAcceptor, listen, acceptor_listen_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixAcceptor, accept, acceptor_accept_arginfo, ZEND_ACC_PUBLIC)
    P3_ME_D(Asio::UnixAcceptor, close, nullptr, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* }}} */

/* {{{ interfaces class_entry */

auto resolver_ce = p3::initClassEntry<void>("Asio\\Resolver", resolver_methods);

auto socket_ce = p3::initClassEntry<void>("Asio\\Socket", socket_methods);

auto inet_socket_ce = p3::initClassEntry<void>("Asio\\InetSocket", inet_socket_methods);

auto local_socket_ce = p3::initClassEntry<void>("Asio\\LocalSocket", local_socket_methods);

auto stream_socket_ce = p3::initClassEntry<void>("Asio\\StreamSocket", stream_socket_methods);

auto datagram_socket_ce = p3::initClassEntry<void>("Asio\\DatagramSocket", datagram_socket_methods);

auto acceptor_ce = p3::initClassEntry<void>("Asio\\Acceptor", acceptor_methods);

/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(asio)
{
    inet_socket_ce->parent = socket_ce;
    local_socket_ce->parent = socket_ce;
    stream_socket_ce->parent = socket_ce;
    datagram_socket_ce->parent = socket_ce;
    p3::initClassEntry<Asio::Service>("Asio\\Service", service_methods);
#ifdef ENABLE_COROUTINE
    p3::initClassEntry<Asio::Future>("Asio\\Future", nullptr);
#endif // ENABLE_COROUTINE
#ifdef ENABLE_STRAND
    p3::initClassEntry<Asio::Strand>("Asio\\Strand", strand_methods);
    p3::initClassEntry<Asio::WrappedHandler>("Asio\\WrappedHandler", wrapped_handler_methods);
#endif // ENABLE_STRAND
    p3::initClassEntry<Asio::Timer>("Asio\\Timer", timer_methods);
    p3::initClassEntry<Asio::Signal>("Asio\\Signal", signal_methods);
    auto ce = p3::initClassEntry<Asio::TcpResolver>("Asio\\TcpResolver", tcp_resolver_methods);
    zend_class_implements(ce, 1, resolver_ce);
    ce = p3::initClassEntry<Asio::UdpResolver>("Asio\\UdpResolver", udp_resolver_methods);
    zend_class_implements(ce, 1, resolver_ce);
    ce = p3::initClassEntry<Asio::TcpSocket>("Asio\\TcpSocket", tcp_socket_methods);
    zend_class_implements(ce, 2, inet_socket_ce, stream_socket_ce);
    ce = p3::initClassEntry<Asio::UdpSocket>("Asio\\UdpSocket", udp_socket_methods);
    zend_class_implements(ce, 2, inet_socket_ce, datagram_socket_ce);
    ce = p3::initClassEntry<Asio::UnixSocket>("Asio\\UnixSocket", unix_socket_methods);
    zend_class_implements(ce, 2, local_socket_ce, stream_socket_ce);
    ce = p3::initClassEntry<Asio::UdgSocket>("Asio\\UdgSocket", udg_socket_methods);
    zend_class_implements(ce, 2, local_socket_ce, datagram_socket_ce);
    ce = p3::initClassEntry<Asio::TcpAcceptor>("Asio\\TcpAcceptor", tcp_acceptor_methods);
    zend_class_implements(ce, 1, acceptor_ce);
    ce = p3::initClassEntry<Asio::UnixAcceptor>("Asio\\UnixAcceptor", unix_acceptor_methods);
    zend_class_implements(ce, 1, acceptor_ce);
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
