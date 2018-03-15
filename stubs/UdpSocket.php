<?php

/**
 * php-asio/stubs/UdpSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class UdpSocket
 *
 * Provides operations on UDP sockets.
 *
 * @package Asio
 */
final class UdpSocket implements InetSocket, DatagramSocket
{
    /**
     * This class can only be instantiated using `Service::addUdpSocket()`.
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function open(bool $inet6) {}

    /**
     * {@inheritdoc}
     */
    function assign(bool $inet6, $native_handle) {}

    /**
     * {@inheritdoc}
     */
    function bind(string $address, int $port) {}

    /**
     * {@inheritdoc}
     */
    function recvFrom(int $length, callable $callback = null, $argument = null) {}

    /**
     * Send asynchronously to UNIX datagram socket.
     *
     * @param string $data : Data to send to datagram socket.
     * @param string $address : Remote address
     * @param int $port : Remote port
     * @param callable $callback[optional]
     * @param mixed $argument
     * @return Future
     */
    function sendTo(string $data, string $address, int $port, callable $callback, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function remoteAddr() {}

    /**
     * {@inheritdoc}
     */
    function remotePort() {}

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
}