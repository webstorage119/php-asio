<?php

/**
 * php-asio/stubs/UdgSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class UdgSocket
 *
 *
 * @package Asio
 */
final class UdgSocket implements DatagramSocket, LocalSocket
{
    /**
     * This class can only be instantiated using `Service::addUdgSocket()`.
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function open() {}

    /**
     * {@inheritdoc}
     */
    function assign($native_handle) {}

    /**
     * {@inheritdoc}
     */
    function bind(string $path) {}

    /**
     * {@inheritdoc}
     */
    function recvFrom(int $length, callable $callback = null, $argument = null) {}

    /**
     * Send asynchronously to UNIX datagram socket.
     *
     * @param string $data : Data to send to datagram socket.
     * @param string $path : Socket path of remote endpoint.
     * @param callable $callback[optional]
     * @param mixed $argument
     * @return Future
     */
    function sendTo(string $data, string $path, callable $callback, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function remotePath() {}

    /**
     * {@inheritdoc}
     */
    function available(int &$ec = null) {}

    /**
     * {@inheritdoc}
     */
    function atMark(int &$ec = null) {}

    /**
     * {@inheritdoc}
     */
    function cancel() {}

    /**
     * {@inheritdoc}
     */
    function close() {}

    /**
     * {@inheritdoc}
     */
    function destroy() {}
}