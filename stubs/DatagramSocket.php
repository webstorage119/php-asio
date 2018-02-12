<?php
/**
 * php-asio/stubs/DatagramSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface DatagramSocket
 *
 * @package Asio
 */
interface DatagramSocket extends Socket
{
    /**
     * Receive asynchronously from datagram socket.
     *
     * @param int $length
     * @param callable $callback[optional] : Receive handler callback
     * @param mixed $argument
     * @return Future
     */
    function recvFrom(int $length, callable $callback, $argument = null);
}