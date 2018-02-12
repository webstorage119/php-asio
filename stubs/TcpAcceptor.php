<?php
/**
 * php-asio/stubs/TcpAcceptor.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class TcpAcceptor
 *
 * Used for accepting new TCP connections.
 *
 * @package Asio
 */
final class TcpAcceptor implements Acceptor
{
    /**
     * This class can only be instantiated using `Service::addTcpAcceptor()`.
     */
    private function __construct() {}

    /**
     * Open acceptor.
     *
     * @param bool $use_ipv6 : True for AF_INET6, false for AF_INET
     * @return int : Error code
     */
    function open(bool $use_ipv6) {}

    /**
     * Assign an existing native socket to the acceptor.
     *
     * @param bool $inet6
     * @param int|resource $native_handle
     * @return int : Error code
     */
    function assign(bool $inet6, $native_handle) {}

    /**
     * Bind acceptor to local endpoint.
     *
     * @param string $address
     * @param int $port
     * @return int : Error code
     */
    function bind(string $address, int $port) {}

    /**
     * {@inheritdoc}
     */
    function listen(int $backlog = null) {}

    /**
     * {@inheritdoc}
     */
    function accept(callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function close() {}
}