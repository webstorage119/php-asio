<?php
/**
 * php-asio/stubs/InetSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface InetSocket
 *
 * @package Asio
 */
interface InetSocket extends Socket
{
    /**
     * Open socket.
     *
     * @param bool $inet6 : True for AF_INET6, false for AF_INET
     * @return int : Error code
     */
    function open(bool $inet6);

    /**
     * Assign an existing native socket to the socket.
     *
     * @param bool $inet6
     * @param int|resource $native_handle
     * @return int : Error code
     */
    function assign(bool $inet6, $native_handle);

    /**
     * Bind socket to a local endpoint.
     *
     * @param string $address
     * @param int $port
     * @return int : Error code
     */
    function bind(string $address, int $port);

    /**
     * Get remote endpoint address.
     *
     * @return string
     */
    function remoteAddr();

    /**
     * Get remote endpoint port.
     *
     * @return mixed
     */
    function remotePort();
}