/**
 * php-asio/acceptor.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "acceptor.hpp"
#include "future.hpp"
#include "stream_descriptor.hpp"

namespace Asio
{
    template <typename Protocol>
    zval* Acceptor<Protocol>::handler(const boost::system::error_code& error,
        Socket<Protocol>* const socket, zval* callback, zval* argument)
    {
        PHP_ASIO_INVOKE_CALLBACK_START(4)
            ZVAL_OBJ(&arguments[1], p3::toZendObject(socket));
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_OBJ, p3::toZendObject(socket));
    }

    /* {{{ proto int TcpAcceptor::accept(bool inet6);
     * Open socket acceptor. */
    template <>
    P3_METHOD(TcpAcceptor, open)
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
    P3_METHOD(UnixAcceptor, open)
    {
        boost::system::error_code ec;
        acceptor_.open(unix(), ec);
        RETVAL_EC(ec)
    }
    /* }}} */

    /* {{{ proto int TcpAcceptor::assign(bool inet6, int|resource fd);
     * Assign an existing native socket to the acceptor. */
    template <>
    P3_METHOD(TcpAcceptor, assign)
    {
        PHP_ASIO_INET_ASSIGN(acceptor_, tcp);
    }
    /* }}} */

    /* {{{ proto int UnixAcceptor::assign(int|resource fd);
     * Assign an existing native socket to the acceptor. */
    template <>
    P3_METHOD(UnixAcceptor, assign)
    {
        PHP_ASIO_LOCAL_ASSIGN(acceptor_, unix);
    }
    /* }}} */

    /* {{{ proto int TcpAcceptor::bind(string address, int port);
     * Bind the acceptor to the specified local endpoint. */
    template <>
    P3_METHOD(TcpAcceptor, bind)
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
    P3_METHOD(UnixAcceptor, bind)
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
    P3_METHOD(Acceptor<Protocol>, listen)
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
    P3_METHOD(Acceptor<Protocol>, accept)
    {
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(0, 2)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_OBJ_ALLOC(socket, Socket<Protocol>, io_service_);
        PHP_ASIO_FUTURE_INIT();
        auto asio_socket = p3::toObject<Socket<Protocol>>(socket);
        future->on_resolve<NOARG>(boost::bind(
            &Acceptor::handler, this, _1, asio_socket, STRAND_UNWRAP(), args));
        acceptor_.async_accept(asio_socket->get_socket(), STRAND_RESOLVE(ASYNC_HANDLER_SINGLE_ARG));
        FUTURE_RETURN();
    }
    /* }}} */

    /* proto int Acceptor::close(void);
     * Cancel async operations and stop acceptor. */
    template <typename Protocol>
    P3_METHOD(Acceptor<Protocol>, close)
    {
        boost::system::error_code ec;
        acceptor_.cancel(ec);
        if (!ec) {
            acceptor_.close(ec);
            if (!ec)
                PHP_ASIO_OBJ_DTOR();
        }
        RETVAL_LONG(static_cast<zend_long>(ec.value()));
    }
    /* }}} */

    template <typename Protocol>
    zend_class_entry* Acceptor<Protocol>::class_entry;

    template <typename Protocol>
    zend_object_handlers Acceptor<Protocol>::handlers;

    template class Acceptor<tcp>;
    template class Acceptor<unix>;
}
