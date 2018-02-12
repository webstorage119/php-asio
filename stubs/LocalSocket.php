<?php
/**
 * php-asio/stubs/LocalSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface LocalSocket
 *
 * @package Asio
 */
interface LocalSocket extends Socket
{
    /**
     * Open socket.
     *
     * @return int : Error code
     */
    function open();

    /**
     * Assign an existing native socket to the socket.
     *
     * @param int|resource $native_handle
     * @return int : Error code
     */
    function assign($native_handle);

    /**
     * Bind socket to a local endpoint.
     *
     * @param string $path : Path to socket file
     * @return mixed
     */
    function bind(string $path);

    /**
     * Get remote endpoint path.
     *
     * @return string
     */
    function remotePath();
}