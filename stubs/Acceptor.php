<?php
/**
 * php-asio/stubs/Acceptor.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface Acceptor
 *
 * @package Asio
 */
interface Acceptor
{
    /**
     * Put the acceptor into the state where it may accept new connections.
     *
     * @param int $backlog[optional] : The maximum length of the queue of pending connections.
     *                                 Default to `boost::asio::socket_base::max_connections`.
     * @return int : Error code
     */
    function listen(int $backlog);

    /**
     * Asynchronously accept a new connection into a socket.
     *
     * @param callable $callback[optional] : Acceptor callback
     * @param mixed $argument
     * @return Future : Resolves Socket.
     */
    function accept(callable $callback, $argument = null);

    /**
     * Cancel async operations and stop acceptor.
     *
     * @return int : Error code
     */
    function close();
}