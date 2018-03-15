<?php
/**
 * php-asio/stubs/Service.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class Service
 *
 * Provide access to instantiation of I/O objects.
 *
 * @package Asio
 */
class Service
{
    /**
     * Add a new timer.
     *
     * @return Timer
     */
    function addTimer() {}

    /**
     * Add a new TCP acceptor.
     *
     * @return TcpAcceptor
     */
    function addTcpAcceptor() {}

    /**
     * Add a new UNIX domain socket acceptor (SOCK_STREAM).
     *
     * @return UnixAcceptor
     */
    function addUnixAcceptor() {}

    /**
     * Add a TCP resolver.
     *
     * @return TcpResolver
     */
    function addTcpResolver() {}

    /**
     * Add a UDP resolver.
     *
     * @return UdpResolver
     */
    function addUdpResolver() {}

    /**
     * Add a new TCP socket.
     *
     * @return TcpSocket
     */
    function addTcpSocket() {}

    /**
     * Add a new UDP socket.
     *
     * @return UdpSocket
     */
    function addUdpSocket() {}

    /**
     * Add a new UNIX domain socket (SOCK_STREAM).
     *
     * @return UnixSocket
     */
    function addUnixSocket() {}

    /**
     * Add a new UNIX domain socket (SOCK_DGRAM).
     *
     * @return UdgSocket
     */
    function addUdgSocket() {}

    /**
     * Add new signal set.
     *
     * @return Signal
     */
    function addSignal() {}

    /**
     * Add new stream descriptor.
     *
     * @return StreamDescriptor
     */
    function addStreamDescriptor() {}

    /**
     * Add new strand.
     *
     * @return Strand
     */
    function addStrand() {}

    /**
     * Run the event loop until stopped or no more work.
     *
     * @param int $ec[optional] : Error code
     * @return int : The number of handlers executed
     */
    function run(int &$ec) {}

    /**
     * Run until stopped or one operation is performed.
     *
     * @param int $ec[optional] : Error code
     * @return int : The number of handlers executed
     */
    function runOne(int &$ec) {}

    /**
     * Poll for operations without blocking.
     *
     * @param int $ec[optional] : Error code
     * @return int : The number of handlers executed
     */
    function poll(int &$ec) {}

    /**
     * Poll for one operation without blocking.
     *
     * @param int $ec[optional] : Error code
     * @return int : The number of handlers executed
     */
    function pollOne(int &$ec) {}

    /**
     * Request invocation of the given handler and return immediately.
     *
     * @param callable $callback
     * @param mixed $argument
     * @return int : The number of handlers executed
     */
    function post(callable $callback, $argument = null) {}

    /**
     * Request invocation of the given handler.
     *
     * @param callable $callback
     * @param mixed $argument
     * @return int : The number of handlers executed
     */
    function dispatch(callable $callback, $argument = null) {}

    /**
     * Notify all services of a fork event. (prepare to fork)
     *
     * @return int : Error code
     */
    function forkPrepare() {}

    /**
     * Notify all services of a fork event. (fork parent)
     *
     * @return int : Error code
     */
    function forkParent() {}

    /**
     * Notify all services of a fork event. (fork child)
     *
     * @return int : Error code
     */
    function forkChild() {}

    /**
     * Stop the event processing loop.
     *
     * @return void
     */
    function stop() {}

    /**
     * Reset in preparation for a subsequent run invocation.
     *
     * @return void
     */
    function reset() {}

    /**
     * Determine whether the io_service is stopped.
     */
    function stopped() {}

    /**
     * Get last error code emitted by yielded async operations of this thread.
     *
     * @return int
     */
    static function lastError() {}
}