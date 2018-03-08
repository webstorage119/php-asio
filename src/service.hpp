/**
 * php-asio/service.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "future.hpp"
#include "timer.hpp"
#include "signal.hpp"
#include "resolver.hpp"
#include "socket.hpp" 
#include "acceptor.hpp"
#include "stream_descriptor.hpp"
#include "strand.hpp"
#include "php_asio.hpp"

namespace asio
{
    /// Wrapper for Boost.Asio io_service.
    /// Provide access to instantiation of I/O objects.
    class service
    {
        /// The io_service of all I/O objects in current instance.
        boost::asio::io_service io_service_;

    public:
        /// Default constructor.
        service() = default;

        /// Deleted copy constructor.
        service(const service&) = delete;

        /// Default destructor.
        virtual ~service() = default;

        /// Deleted copy assignment operator.
        service& operator=(const service&) = delete;

        /* {{{ proto Timer Service::addTimer(void);
         * Add a new timer. */
        P3_METHOD_DECLARE(addTimer);
        /* }}} */

        /* {{{ proto Signal Service::addSignal(void);
         * Add a new signal set. */
        P3_METHOD_DECLARE(addSignal);
        /* }}} */

        /* {{{ proto TcpResolver Service::addTcpResolver(void);
         * Add a new TCP resolver. */
        P3_METHOD_DECLARE(addTcpResolver);
        /* }}} */

        /* {{{ proto UdpResolver Service::addUdpResolver(void);
         * Add a new UDP resolver.*/
        P3_METHOD_DECLARE(addUdpResolver);
        /* }}} */

        /* {{{ proto TcpSocket Service::addUdpSocket(void);
         * Add a new TCP socket. */
        P3_METHOD_DECLARE(addTcpSocket);
        /* }}} */

        /* {{{ proto UdpSocket Service::addUdpSocket(void);
         * Add a new UDP socket. */
        P3_METHOD_DECLARE(addUdpSocket);
        /* }}} */

        /* {{{ proto UnixSocket Service::addUnixSocket(void);
         * Add a new UNIX domain socket (SOCK_STREAM). */
        P3_METHOD_DECLARE(addUnixSocket);
        /* }}} */

        /* {{{ proto UdgSocket Service::addUdgSocket(void);
         * Add a new UNIX domain socket (SOCK_DGRAM). */
        P3_METHOD_DECLARE(addUdgSocket);
        /* }}} */

        /* {{{ proto TcpAcceptor Service::addTcpAcceptor(void);
         * Add a new TCP acceptor. */
        P3_METHOD_DECLARE(addTcpAcceptor);
        /* }}} */

        /* {{{ proto UnixAcceptor Service::addUnixAcceptor(void);
         * Add a new UNIX domain socket acceptor (SOCK_STREAM). */
        P3_METHOD_DECLARE(addUnixAcceptor);
        /* }}} */

        /* {{{ proto StreamDescriptor Service::addStreamDescriptor(void);
         * Add a new stream descriptor. */
        P3_METHOD_DECLARE(addStreamDescriptor);
        /* }}} */

#ifdef ENABLE_STRAND
        /* {{{ proto Strand Service::addStrand(void);
         * Add a new strand object. */
        P3_METHOD_DECLARE(addStrand);
        /* }}} */
#endif // ENABLE_STRAND

        /* {{{ proto int run([int &ec]);
         * Run the event loop until stopped or no more work. */
        P3_METHOD_DECLARE(run);
        /* }}} */

        /* {{{ proto int runOne([int &ec]);
         * Run until stopped or one operation is performed. */
        P3_METHOD_DECLARE(runOne);
        /* }}} */

        /* {{{ proto int poll([int &ec]);
         * Poll for operations without blocking. */
        P3_METHOD_DECLARE(poll);
        /* }}} */

        /* {{{ proto int pollOne([int &ec]);
         * Poll for one operation without blocking. */
        P3_METHOD_DECLARE(pollOne);
        /* }}} */

        /* {{{ proto void stop(void);
         * Stop the event processing loop. */
        P3_METHOD_DECLARE(stop);
        /* }}} */

        /* {{{ proto void stop(void);
         * Reset in preparation for a subsequent run invocation. */
        P3_METHOD_DECLARE(reset);
        /* }}} */

        /* {{{ proto bool stopped(void);
         * Determine whether the io_service is stopped. */
        P3_METHOD_DECLARE(stopped) const;
        /* }}} */

        /* {{{ proto void Service::post(callable callback, [mixed argument]);
         * Request invocation of the given handler and return immediately. */
        P3_METHOD_DECLARE(post);
        /* }}} */

        /* {{{ proto void Service::dispatch(callable callback, [mixed argument]);
         * Request invocation of the given handler. */
        P3_METHOD_DECLARE(dispatch);

        /* {{{ proto int Service::forkPrepare(void);
         * Notify all services of a fork event. (prepare to fork) */
        P3_METHOD_DECLARE(forkPrepare);
        /* }}} */

        /* {{{ proto int Service::forkParent(void);
        * Notify all services of a fork event. (fork parent) */
        P3_METHOD_DECLARE(forkParent);
        /* }}} */

        /* {{{ proto int Service::forkChild(void);
         * Notify all services of a fork event. (fork child) */
        P3_METHOD_DECLARE(forkChild);

        /// Get io_service by reference.
        /// Can be used when working on another extension based on Boost.Asio.
        PHP_ASIO_API boost::asio::io_service& get_io_service()
        {
            return io_service_;
        }

        PHP_ASIO_CE_DECLARE();
    };
}
