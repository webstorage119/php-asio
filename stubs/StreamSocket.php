<?php
/**
 * php-asio/stubs/StreamSocket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface StreamSocket
 *
 * @package Asio
 */
interface StreamSocket extends Socket
{
    /**
     * Read asynchronously from stream socket.
     *
     * @param int $length : Max number of bytes to be read
     * @param bool $read_some
     * @param callable $callback[optional] : Read handler callback
     * @param mixed $argument
     * @return Future : Resolves received data(string)
     */
    function read(int $length, bool $read_some = true, callable $callback, $argument = null);

    /**
     * Write asynchronously to stream socket.
     *
     * @param string $data : Write buffer
     * @param bool $write_some
     * @param callable $callback[optional] : Write handler callback
     * @param mixed $argument
     * @return Future : Resolves bytes transferred(int)
     */
    function write(string $data, bool $write_some = false, callable $callback, $argument = null);
}