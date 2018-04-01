/**
 * php-asio/socket.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "socket.hpp"
#include "future.hpp"
#include "io.hpp"

namespace asio
{
    template <typename Protocol>
    zval* socket<Protocol>::connect_handler(const boost::system::error_code& error,
        zval* callback, zval* argument)
    {
        PHP_ASIO_INVOKE_CALLBACK_START(3)
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN_NULL();
    }

    template <typename Protocol> template <typename, typename>
    zval* socket<Protocol>::read_handler(const boost::system::error_code& error,
        size_t length, zend_string* buffer, zval* callback, zval* argument)
    {
        // Zend strings have to be zero-terminated.
        // Otherwise you'll get E_WARNING on zval dtor.
        ZSTR_VAL(buffer)[length] = '\0';
        ZSTR_LEN(buffer) = length;
        PHP_ASIO_INVOKE_CALLBACK_START(4)
            ZVAL_STR(&arguments[1], buffer);
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_STR, buffer);
    }

    template <typename Protocol>
    zval* socket<Protocol>::write_handler(const boost::system::error_code& error,
        size_t length, zend_string* buffer, zval* callback, zval* argument)
    {
#ifdef ENABLE_NULL_BUFFERS
        if (buffer)
#endif //ENABLE_NULL_BUFFERS
            zend_string_release(buffer);
        PHP_ASIO_INVOKE_CALLBACK_START(4)
            ZVAL_LONG(&arguments[1], static_cast<zend_long>(length));
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_LONG, static_cast<zend_long>(length));
    }

    template <>
    zval* udp_socket::recv_handler(const boost::system::error_code& error,
        size_t length, zend_string* buffer, udp::endpoint* endpoint,
        zval* callback, zval* argument)
    {
        ZSTR_VAL(buffer)[length] = '\0';
        ZSTR_LEN(buffer) = length;
#ifdef ENABLE_COROUTINE
        last_addr_<> = endpoint->address().to_string();
        last_port_<> = endpoint->port();
#endif // ENABLE_COROUTINE
        PHP_ASIO_INVOKE_CALLBACK_START(6)
            ZVAL_STR(&arguments[1], buffer);
#ifdef ENABLE_COROUTINE
            ZVAL_STRING(&arguments[2], last_addr_<>.data());
            ZVAL_LONG(&arguments[3], static_cast<zend_long>(last_port_<>));
#else
            ZVAL_STRING(&arguments[2], endpoint->address().to_string().data());
            ZVAL_LONG(&arguments[3], static_cast<zend_long>(endpoint->port()));
#endif // ENABLE_COROUTINE
            PHP_ASIO_INVOKE_CALLBACK();
            zval_ptr_dtor(&arguments[2]);
        PHP_ASIO_INVOKE_CALLBACK_END();
        delete endpoint;
        CORO_RETURN(ZVAL_STR, buffer);
    }

    template <>
    zval* udg_socket::recv_handler(const boost::system::error_code& error,
        size_t length, zend_string* buffer, udg::endpoint* endpoint,
        zval* callback, zval* argument)
    {
        ZSTR_VAL(buffer)[length] = '\0';
        ZSTR_LEN(buffer) = length;
#ifdef ENABLE_COROUTINE
        last_path_<> = endpoint->path();
#endif // ENABLE_COROUTINE
        PHP_ASIO_INVOKE_CALLBACK_START(5)
            ZVAL_STR(&arguments[1], buffer);
#ifdef ENABLE_COROUTINE
            ZVAL_STRING(&arguments[2], last_path_<>.data());
#else
            ZVAL_STRING(&arguments[2], endpoint->path().data());
#endif // ENABLE_COROUTINE
            PHP_ASIO_INVOKE_CALLBACK();
            zval_ptr_dtor(&arguments[2]);
        PHP_ASIO_INVOKE_CALLBACK_END();
        delete endpoint;
        CORO_RETURN(ZVAL_STR, buffer);
    }

    template <typename Protocol> template <typename P, ENABLE_IF_INET(P)>
    P3_METHOD(socket<Protocol>, open_inet)
    {
        zend_bool inet6;
        ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_BOOL(inet6)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        socket_.open(inet6 ? Protocol::v6() : Protocol::v4(), ec);
        RETVAL_EC(ec);
    }

    template <typename Protocol> template <typename P, ENABLE_IF_LOCAL(P)>
    P3_METHOD(socket<Protocol>, open_local)
    {
        boost::system::error_code ec;
        socket_.open(Protocol(), ec);
        RETVAL_EC(ec);
    }

    template <typename Protocol> template <typename P, ENABLE_IF_INET(P)>
    P3_METHOD(socket<Protocol>, assign_inet)
    {
        PHP_ASIO_INET_ASSIGN(socket_, Protocol);
    }

    template <typename Protocol> template <typename P, ENABLE_IF_LOCAL(P)>
    P3_METHOD(socket<Protocol>, assign_local)
    {
        PHP_ASIO_LOCAL_ASSIGN(socket_, Protocol);
    }

    template <typename Protocol> template <typename P, ENABLE_IF_INET(P)>
    P3_METHOD(socket<Protocol>, bind_inet)
    {
        zend_string* address;
        zend_long port_num;
        ZEND_PARSE_PARAMETERS_START(2, 2);
            Z_PARAM_STR(address)
            Z_PARAM_LONG(port_num)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        socket_.bind({ boost::asio::ip::address::from_string(ZSTR_VAL(address)),
            static_cast<unsigned short>(port_num) }, ec);
        RETVAL_EC(ec);
    }

    template <typename Protocol> template <typename P, ENABLE_IF_LOCAL(P)>
    P3_METHOD(socket<Protocol>, bind_local)
    {
        zend_string* socket_path;
        ZEND_PARSE_PARAMETERS_START(1, 1);
            Z_PARAM_STR(socket_path)
        ZEND_PARSE_PARAMETERS_END();
        namespace fs = boost::filesystem;
        if (fs::status(ZSTR_VAL(socket_path)).type() == fs::socket_file)
            fs::remove(ZSTR_VAL(socket_path));
        boost::system::error_code ec;
        socket_.bind({ ZSTR_VAL(socket_path) }, ec);
        RETVAL_EC(ec);
    }

    /* {{{ proto Future TcpSocket::connect(string address, int port,
     *               [callable callback], [mixed argument]);
     * Asynchronously connect to a remote endpoint. */
    template <>
    P3_METHOD(tcp_socket, connect)
    {
        zend_string* address;
        zend_long port;
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(2, 4)
            Z_PARAM_STR(address)
            Z_PARAM_LONG(port)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_FUTURE_INIT();
        future->on_resolve<NOARG>(boost::bind(&socket::connect_handler, this, _1, cb, args));
        socket_.async_connect({ boost::asio::ip::address::from_string(ZSTR_VAL(address)),
            static_cast<unsigned short>(port) }, STRAND_RESOLVE(ASYNC_HANDLER_SINGLE_ARG));
        FUTURE_RETURN();
    }
    /* }}} */

    /* {{{ proto Future UnixSocket::connect(string path, [callable callback], [mixed argument]);
     * Asynchronously connect to a remote endpoint. */
    template <>
    P3_METHOD(unix_socket, connect)
    {
        zend_string* path;
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(1, 3)
            Z_PARAM_STR(path)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_FUTURE_INIT();
        future->on_resolve<NOARG>(boost::bind(&socket::connect_handler, this, _1, cb, args));
        socket_.async_connect({ ZSTR_VAL(path) }, STRAND_RESOLVE(ASYNC_HANDLER_SINGLE_ARG));
        FUTURE_RETURN();
    }
    /* }}} */

    template <typename Protocol> template <typename, typename>
    P3_METHOD(socket<Protocol>, read)
    {
        PHP_ASIO_READ(socket, socket_);
    }

    template <typename Protocol> template <typename, typename>
    P3_METHOD(socket<Protocol>, write)
    {
        PHP_ASIO_WRITE(socket, socket_);
    }

    template <typename Protocol> template <typename, typename>
    P3_METHOD(socket<Protocol>, recvFrom)
    {
        zend_long length;
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(1, 3);
            Z_PARAM_LONG(length)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_BUFFER_LEN_VALIDATE();
        auto buffer_container =
#ifdef ENABLE_NULL_BUFFERS
            length == 0 ? ZSTR_EMPTY_ALLOC() :
#endif //ENABLE_NULL_BUFFERS
            zend_string_alloc(static_cast<size_t>(length), 0);
        auto endpoint = new typename Protocol::endpoint;
        PHP_ASIO_FUTURE_INIT();
        future->template on_resolve<size_t>(boost::bind(&socket::recv_handler,
            this, _1, _2, buffer_container, endpoint, cb, args));
#ifdef ENABLE_NULL_BUFFERS
        if (length == 0)
            socket_.async_receive_from(boost::asio::null_buffers(),
                *endpoint, STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t)));
        else
#endif // ENABLE_NULL_BUFFERS
            socket_.async_receive_from(mutable_buffer(buffer_container),
                *endpoint, STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t)));
        FUTURE_RETURN();
    }

    /* {{{ proto Future UdpSocket::sendTo(string data, string address, int port,
     *               [callable callback], [mixed argument]);
    * Send asynchronously to UDP socket. */
    template <> template <>
    P3_METHOD(udp_socket, sendTo)
    {
        zend_string* data;
        zend_string* address;
        zend_long port;
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(3, 5)
            Z_PARAM_STR(data)
            Z_PARAM_STR(address)
            Z_PARAM_LONG(port)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        const udp::endpoint endpoint(boost::asio::ip::address::from_string(ZSTR_VAL(address)),
            static_cast<unsigned short>(port));
        const auto buffer_container =
#ifdef ENABLE_NULL_BUFFERS
            ZSTR_LEN(data) == 0 ? nullptr :
#endif //ENABLE_NULL_BUFFERS
            zend_string_copy(data);
        PHP_ASIO_FUTURE_INIT();
        future->on_resolve<size_t>(boost::bind(&socket::write_handler,
            this, _1, _2, buffer_container, cb, args));
#ifdef ENABLE_NULL_BUFFERS
        if (ZSTR_LEN(data) == 0)
            socket_.async_send_to(boost::asio::null_buffers(),
                endpoint, STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t)));
        else
#endif // ENABLE_NULL_BUFFERS
            socket_.async_send_to(const_buffer(buffer_container),
                endpoint, STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t)));
        FUTURE_RETURN();
    }
    /* }}} */

    /* {{{ proto Future UdgSocket::sendTo(string data, string path,
     *               [callable callback], [mixed argument]);
     * Send asynchronously to UNIX datagram socket. */
    template <> template <>
    P3_METHOD(udg_socket, sendTo)
    {
        zend_string* data;
        zend_string* path;
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(2, 4)
            Z_PARAM_STR(data)
            Z_PARAM_STR(path)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        const auto buffer_container =
#ifdef ENABLE_NULL_BUFFERS
            ZSTR_LEN(data) == 0 ? nullptr :
#endif //ENABLE_NULL_BUFFERS
            zend_string_copy(data);
        PHP_ASIO_FUTURE_INIT();
        future->on_resolve<size_t>(boost::bind(
            &socket::write_handler, this, _1, _2, buffer_container, cb, args));
#ifdef ENABLE_NULL_BUFFERS
        if (ZSTR_LEN(data) == 0)
            socket_.async_send_to(boost::asio::null_buffers(), { ZSTR_VAL(path) },
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t)));
        else
#endif //ENABLE_NULL_BUFFERS
            socket_.async_send_to(const_buffer(buffer_container), { ZSTR_VAL(path) },
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t)));
        FUTURE_RETURN();
    }
    /* }}} */

    template <> template <>
    P3_METHOD(tcp_socket, remoteAddr) const
    {
        RETVAL_STRING(socket_.remote_endpoint().address().to_string().c_str());
    }

    template <> template <>
    P3_METHOD(tcp_socket, remotePort) const
    {
        RETVAL_LONG(static_cast<zend_long>(socket_.remote_endpoint().port()));
    }

    template <> template <>
    P3_METHOD(unix_socket, remotePath) const
    {
        RETVAL_STRING(socket_.remote_endpoint().path().c_str());
    }

    template <> template <>
    P3_METHOD(udp_socket, remoteAddr) const
    {
#ifdef ENABLE_COROUTINE
        RETVAL_STRING(last_addr_<>.data());
#else
        PHP_ASIO_ERROR(E_WARNING, "Not available without coroutine support.");
        RETVAL_NULL();
#endif // ENABLE_COROUTINE
    }

    template <> template <>
    P3_METHOD(udp_socket, remotePort) const
    {
#ifdef ENABLE_COROUTINE
        RETVAL_LONG(static_cast<zend_long>(last_port_<>));
#else
        PHP_ASIO_ERROR(E_WARNING, "Not available without coroutine support.");
        RETVAL_NULL();
#endif // ENABLE_COROUTINE
    }

    template <> template <>
    P3_METHOD(udg_socket, remotePath) const
    {
#ifdef ENABLE_COROUTINE
        RETVAL_STRING(last_path_<>.data());
#else
        PHP_ASIO_ERROR(E_WARNING, "Not available without coroutine support.");
        RETVAL_NULL();
#endif // ENABLE_COROUTINE
    }

    template <typename Protocol>
    P3_METHOD(socket<Protocol>, available) const
    {
        zval* error = nullptr;
        ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(error)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        auto bytes = socket_.available(ec);
        if (error) {
            ZVAL_DEREF(error);
            ZVAL_LONG(error, static_cast<zend_long>(ec.value()));
        }
        RETVAL_LONG(static_cast<zend_long>(bytes));
    }

    template <typename Protocol>
    P3_METHOD(socket<Protocol>, atMark) const
    {
        zval* error = nullptr;
        ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(error)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        auto at_mark = socket_.at_mark(ec);
        if (error) {
            ZVAL_DEREF(error);
            ZVAL_LONG(error, static_cast<zend_long>(ec.value()));
        }
        RETVAL_BOOL(at_mark);
    }

    template <typename Protocol>
    P3_METHOD(socket<Protocol>, cancel)
    {
        boost::system::error_code ec;
        RETVAL_EC(socket_.cancel(ec));
    }

    template <typename Protocol>
    P3_METHOD(socket<Protocol>, close)
    {
        boost::system::error_code ec;
        RETVAL_EC(socket_.close(ec));
    }

    template <typename Protocol>
    zend_class_entry* socket<Protocol>::class_entry;

    template <typename Protocol>
    zend_object_handlers socket<Protocol>::handlers;

#ifdef ENABLE_COROUTINE
    template <typename Protocol> template <typename, typename>
    thread_local std::string socket<Protocol>::last_addr_;

    template <typename Protocol> template <typename, typename>
    thread_local unsigned short socket<Protocol>::last_port_;

    template <typename Protocol> template <typename, typename>
    thread_local std::string socket<Protocol>::last_path_;
#endif // ENABLE_COROUTINE

    template class socket<tcp>;
    template P3_METHOD(tcp_socket, open_inet);
    template P3_METHOD(tcp_socket, assign_inet);
    template P3_METHOD(tcp_socket, bind_inet);
    template zval* tcp_socket::read_handler(const boost::system::error_code&,
        size_t, zend_string*, zval*, zval*);
    template P3_METHOD(tcp_socket, read);
    template P3_METHOD(tcp_socket, write);

    template class socket<unix>;
    template P3_METHOD(unix_socket, open_local);
    template P3_METHOD(unix_socket, assign_local);
    template P3_METHOD(unix_socket, bind_local);
    template zval* unix_socket::read_handler(const boost::system::error_code&,
        size_t, zend_string*, zval*, zval*);
    template P3_METHOD(unix_socket, read);
    template P3_METHOD(unix_socket, write);

    template class socket<udp>;
    template P3_METHOD(udp_socket, open_inet);
    template P3_METHOD(udp_socket, assign_inet);
    template P3_METHOD(udp_socket, bind_inet);
    template P3_METHOD(udp_socket, recvFrom);

    template class socket<udg>;
    template P3_METHOD(udg_socket, open_local);
    template P3_METHOD(udg_socket, assign_local);
    template P3_METHOD(udg_socket, bind_local);
    template P3_METHOD(udg_socket, recvFrom);
}
