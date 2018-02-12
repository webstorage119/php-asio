<?php
/**
 * php-asio/stubs/TcpSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class TcpSocket
 *
 * Provides operations on TCP sockets.
 *
 * @package Asio
 */
final class TcpSocket implements StreamSocket, InetSocket
{
    /**
     * This class can only be instantiated using `Service::addTcpSocket()` and `TcpAcceptor::Accept()`.
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
     * Connect to a remote endpoint.
     *
     * @param string $address : Remote address
     * @param int $port : Remote port
     * @param callable $callback[optional]
     * @param mixed $argument
     */
    function connect(string $address, int $port, callable $callback, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function read(int $length, bool $read_some = true, callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function write(string $data, bool $write_some = false, callable $callback = null, $argument = null) {}

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
    function close() {}
}