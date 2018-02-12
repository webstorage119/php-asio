<?php
/**
 * php-asio/stubs/UnixAcceptor.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class UnixAcceptor
 *
 * Used for accepting new UNIX domain socket connections.
 *
 * @package Asio
 */
final class UnixAcceptor implements Acceptor
{
    /**
     * This class can only be instantiated using `Service::addUnixAcceptor()`.
     */
    private function __construct() {}

    /**
     * Open acceptor.
     *
     * @return int : Error code.
     */
    function open() {}

    /**
     * Assign an existing native socket to the acceptor.
     *
     * @param int|resource $native_handle
     * @return int : Error code
     */
    function assign($native_handle) {}

    /**
     * Bind acceptor to local endpoint.
     *
     * @param string $path
     * @return int : Error code.
     */
    function bind(string $path) {}

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