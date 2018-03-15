<?php
/**
 * php-asio/stubs/UnixSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class UnixSocket
 *
 * Provides operations on UNIX domain sockets (SOCK_STREAM).
 *
 * @package Asio
 */
final class UnixSocket implements StreamSocket, LocalSocket
{
    /**
     * This class can only be instantiated using `Service::addUnixSocket()` and `UnixAcceptor::Accept()`.
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
     * Connect to a remote endpoint.
     *
     * @param string $path : Remote socket file path
     * @param callable $callback[optional]
     * @param mixed $argument
     * @return int : Error code
     */
    function connect(string $path, callable $callback, $argument = null) {}

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
}