/**
 * php-asio/acceptor.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "acceptor.hpp"
#include "future.hpp"
#include "io.hpp"

namespace asio
{
    template <typename Protocol>
    zval* acceptor<Protocol>::handler(const boost::system::error_code& error,
        socket<Protocol>* const socket, zval* callback, zval* argument)
    {
        PHP_ASIO_INVOKE_CALLBACK_START(4)
            ZVAL_OBJ(&arguments[1], p3::to_zend_object(socket));
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_OBJ, p3::to_zend_object(socket));
    }

    /* {{{ proto int TcpAcceptor::accept(bool inet6);
     * Open socket acceptor. */
    template <>
    P3_METHOD(tcp_acceptor, open)
    {
        zend_bool inet6;
        ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_BOOL(inet6)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        acceptor_.open(inet6 ? tcp::v6() : tcp::v4(), ec);
        RETVAL_EC(ec)
    }
    /* }}} */

    /* {{{ proto int UnixAcceptor::accept(void);
     * Open socket acceptor. */
    template <>
    P3_METHOD(unix_acceptor, open)
    {
        boost::system::error_code ec;
        acceptor_.open(unix(), ec);
        RETVAL_EC(ec)
    }
    /* }}} */

    /* {{{ proto int TcpAcceptor::assign(bool inet6, int|resource fd);
     * Assign an existing native socket to the acceptor. */
    template <>
    P3_METHOD(tcp_acceptor, assign)
    {
        PHP_ASIO_INET_ASSIGN(acceptor_, tcp);
    }
    /* }}} */

    /* {{{ proto int UnixAcceptor::assign(int|resource fd);
     * Assign an existing native socket to the acceptor. */
    template <>
    P3_METHOD(unix_acceptor, assign)
    {
        PHP_ASIO_LOCAL_ASSIGN(acceptor_, unix);
    }
    /* }}} */

    /* {{{ proto int TcpAcceptor::bind(string address, int port);
     * Bind the acceptor to the specified local endpoint. */
    template <>
    P3_METHOD(tcp_acceptor, bind)
    {
        zend_string* address;
        zend_long port_num;
        ZEND_PARSE_PARAMETERS_START(2, 2);
            Z_PARAM_STR(address)
            Z_PARAM_LONG(port_num)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        acceptor_.bind({ boost::asio::ip::address::from_string(ZSTR_VAL(address)),
            static_cast<unsigned short>(port_num) }, ec);
        RETVAL_EC(ec);
    }
    /* }}} */

    /* {{{ proto int TcpAcceptor::bind(string path);
     * Bind the acceptor to the specified local endpoint. */
    template <>
    P3_METHOD(unix_acceptor, bind)
    {
        zend_string* socket_path;
        ZEND_PARSE_PARAMETERS_START(1, 1);
            Z_PARAM_STR(socket_path)
        ZEND_PARSE_PARAMETERS_END();
        namespace fs = boost::filesystem;
        if (fs::status(ZSTR_VAL(socket_path)).type() == fs::socket_file)
            fs::remove(ZSTR_VAL(socket_path));
        boost::system::error_code ec;
        acceptor_.bind({ ZSTR_VAL(socket_path) }, ec);
        RETVAL_EC(ec);
    }
    /* }}} */

    /* {{{ proto int Acceptor::listen([int backlog]);
     * Put the acceptor into the state where it may accept new connections. */
    template <typename Protocol>
    P3_METHOD(acceptor<Protocol>, listen)
    {
        zend_long backlog = 0;
        ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_LONG(backlog)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        acceptor_.listen(backlog ? static_cast<int>(backlog) :
            Protocol::socket::max_connections, ec);
        RETVAL_EC(ec);
    }
    /* }}} */

    /* {{{ proto int Acceptor::accept([callable callback], [mixed argument]);
     * Asynchronously accept a new connection into a socket. */
    template <typename Protocol>
    P3_METHOD(acceptor<Protocol>, accept)
    {
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(0, 2)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_OBJ_ALLOC(accepted_socket, socket<Protocol>, io_service_);
        PHP_ASIO_FUTURE_INIT();
        auto asio_socket = p3::to_object<socket<Protocol>>(accepted_socket);
        future->template on_resolve<NOARG>(boost::bind(
            &acceptor::handler, this, _1, asio_socket, cb, args));
        acceptor_.async_accept(asio_socket->get_socket(), STRAND_RESOLVE(ASYNC_HANDLER_SINGLE_ARG));
        FUTURE_RETURN();
    }
    /* }}} */

    /* {{{ proto int Acceptor::cancel(void);
     * Cancel all asynchronous operations on this acceptor. */
    template <typename Protocol>
    P3_METHOD(acceptor<Protocol>, cancel)
    {
        boost::system::error_code ec;
        RETVAL_EC(acceptor_.cancel(ec));
    }
    /* }}} */

    /* {{{ proto int Acceptor::close(void);
     * Stop the acceptor. */
    template <typename Protocol>
    P3_METHOD(acceptor<Protocol>, close)
    {
        boost::system::error_code ec;
        RETVAL_EC(acceptor_.close(ec));
    }
    /* }}} */

    template <typename Protocol>
    zend_class_entry* acceptor<Protocol>::class_entry;

    template <typename Protocol>
    zend_object_handlers acceptor<Protocol>::handlers;

    template class acceptor<tcp>;
    template class acceptor<unix>;
}
