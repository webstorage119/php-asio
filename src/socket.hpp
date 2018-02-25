/**
 * php-asio/include/socket.hpp
 * 
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

#include <boost/filesystem.hpp>

#define ENABLE_IF_INET(P) std::string(P::endpoint::*port)()
#define ENABLE_IF_LOCAL(P) std::string(P::endpoint::*path)()

namespace Asio
{
    /**
     * Wrapper for Boost.Asio socket.
     */
    template <typename Protocol>
    class Socket : public Base
    {
        /// Boost.Asio socket instance.
        typename Protocol::socket socket_;

        template <typename T1, typename T2>
        using enable_if_same = std::enable_if_t<std::is_same<T1, T2>::value>;

#ifdef ENABLE_COROUTINE
        /// Address of client who sent the last request.
        template <typename P = Protocol, typename = enable_if_same<P, udp>>
        static thread_local std::string last_addr_;

        /// Port of client who sent the last request.
        template <typename P = Protocol, typename = enable_if_same<P, udp>>
        static thread_local unsigned short last_port_;

        /// Socket path of client who sent the last request.
        template <typename P = Protocol, typename = enable_if_same<P, udg>>
        static thread_local std::string last_path_;
#endif // ENABLE_COROUTINE

        /**
         * Connect handler.
         * @param error : Error code
         */
        zval* connect_handler(const boost::system::error_code& error,
            zval* callback, zval* argument);

        /**
         * Read handler for stream socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param buffer : Read buffer
         */
        template <typename P = Protocol, typename =
            enable_if_same<boost::asio::basic_stream_socket<P>, typename P::socket>>
        zval* read_handler(const boost::system::error_code& error,
            size_t length, zend_string* buffer, zval* callback, zval* argument);

        /**
         * Write/send handler for socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param buffer : Write buffer
         */
        zval* write_handler(const boost::system::error_code& error,
            size_t length, zend_string* buffer, zval* callback, zval* argument);

        /**
         * Receive handler for datagram socket.
         * @param error : Error code
         * @param length : Bytes transferred
         * @param buffer : Write buffer
         */
        zval* recv_handler(const boost::system::error_code& error, size_t length,
            zend_string* buffer, typename Protocol::endpoint* endpoint,
            zval* callback, zval* argument);

    public:
        /**
         * Constructor.
         * @param io_service : I/O service of current socket.
         */
        explicit Socket(
            boost::asio::io_service& io_service
        ) : Base(io_service), socket_(io_service) {}

        /**
         * Get reference of socket.
         */
        typename Protocol::socket& get_socket()
        {
            return socket_;
        }

        template <typename P>
        using inet_p = typename P::endpoint::port;

        /* {{{ proto int InetSocket::open(bool inet6);
         * Open INET socket (AF_INET or AF_INET6). */
        template <typename P = Protocol, ENABLE_IF_INET(P) = nullptr>
        P3_METHOD_DECLARE(open_inet);
        /* }}} */

        /* {{{ proto int LocalSocket::open(void);
         * Open UNIX domain socket. */
        template <typename P = Protocol, ENABLE_IF_LOCAL(P) = nullptr>
        P3_METHOD_DECLARE(open_local);
        /* }}} */

        /* {{{ proto int InetSocket::assign(bool inet6, int|resource fd);
         * Assign an existing native socket to the socket. */
        template <typename P = Protocol, ENABLE_IF_INET(P) = nullptr>
        P3_METHOD_DECLARE(assign_inet);
        /* }}} */

        /* {{{ proto int LocalSocket::assign(int|resource fd);
         * Assign an existing native socket to the socket. */
        template <typename P = Protocol, ENABLE_IF_LOCAL(P) = nullptr>
        P3_METHOD_DECLARE(assign_local);
        /* }}} */

        /* {{{ proto int InetSocket::bind(string address, int port);
         * Bind socket to a local endpoint (AF_INET or AF_INET6). */
        template <typename P = Protocol, ENABLE_IF_INET(P) = nullptr>
        P3_METHOD_DECLARE(bind_inet);
        /* }}} */

        /* {{{ proto int LocalSocket::bind(string path);
         * Bind socket to a local endpoint (AF_UNIX). */
        template <typename P = Protocol, ENABLE_IF_LOCAL(P) = nullptr>
        P3_METHOD_DECLARE(bind_local);
        /* }}} */

        P3_METHOD_DECLARE(connect);

        /* {{{ proto Future StreamSocket::read(int length, [bool read_some = true],
         *               [callable callback], [mixed argument]);
         * Read asynchronously from stream socket. */
        template <typename P = Protocol, typename = 
            enable_if_same<boost::asio::basic_stream_socket<P>, typename P::socket>>
        P3_METHOD_DECLARE(read);
        /* }}} */

        /* {{{ proto Future StreamSocket::write(string data, [bool write_some = false],
         *               [callable callback], [mixed argument]);
         * Write asynchronously to stream socket. */
        template <typename P = Protocol, typename =
            enable_if_same<boost::asio::basic_stream_socket<P>, typename P::socket>>
        P3_METHOD_DECLARE(write);
        /* }}} */

        /* {{{ proto Future DatagramSocket::recvFrom(int length,
         *               [callable callback], [mixed argument]);
         * Receive asynchronously from datagram socket.*/
        template <typename P = Protocol, typename = 
            enable_if_same<boost::asio::basic_datagram_socket<P>, typename P::socket>>
        P3_METHOD_DECLARE(recvFrom);
        /* }}} */

        template <typename P = Protocol, typename =
            enable_if_same<boost::asio::basic_datagram_socket<P>, typename P::socket>>
        P3_METHOD_DECLARE(sendTo);

        /* {{{ proto string InetSocket::remoteAddr(void);
         * Get remote address (AF_INET or AF_INET6). */
        template <typename P = Protocol, ENABLE_IF_INET(P) = nullptr>
        P3_METHOD_DECLARE(remoteAddr) const;
        /* }}} */

        /* {{{ proto int InetSocket::remotePort(void);
         * Get remote port (AF_INET or AF_INET6). */
        template <typename P = Protocol, ENABLE_IF_INET(P) = nullptr>
        P3_METHOD_DECLARE(remotePort) const;
        /* }}} */

        /* {{{ proto string LocalSocket::remotePath(void);
         * Get socket file path of remote endpoint (AF_UNIX). */
        template <typename P = Protocol, ENABLE_IF_LOCAL(P) = nullptr>
        P3_METHOD_DECLARE(remotePath) const;
        /* }}} */

        /* {{{ proto int Socket::available([int &ec]);
         * Determine the number of bytes available for reading. */
        P3_METHOD_DECLARE(available) const;
        /* }}} */

        /* {{{ proto bool Socket::atMark([int &ec]);
         * Determine whether the socket is at the out-of-band data mark. */
        P3_METHOD_DECLARE(atMark) const;
        /* }}} */

        /* {{{ proto int Socket::close(void);
         * Cancel async operations and close socket. */
        P3_METHOD_DECLARE(close);
        /* }}} */

        PHP_ASIO_CE_DECLARE();
    };

    using TcpSocket = Socket<tcp>;
    using UnixSocket = Socket<unix>;
    using UdpSocket = Socket<udp>;
    using UdgSocket = Socket<udg>;
}
