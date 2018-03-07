<?php
/**
 * php-asio/stubs/StreamDescriptor.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class StreamDescriptor
 *
 * Provides stream-oriented descriptor functionality.
 *
 * @package Asio
 */
final class StreamDescriptor implements IoObject
{
    /**
     * This class can only be instantiated using `Service::addStreamDescriptor()`.
     */
    private function __construct() {}

    /**
     * Opens the descriptor to hold an existing native descriptor.
     *
     * @param int|resource $native_handle
     * @return int : Error code
     */
    function assign($native_handle) {}

    /**
     * Determine whether the descriptor is open.
     *
     * @return bool
     */
    function isOpen() {}

    /**
     * Read asynchronously from stream descriptor.
     *
     * @param int $length : Max number of bytes to be read
     * @param bool $read_some
     * @param callable $callback[optional] : Read handler callback
     * @param mixed $argument
     * @return Future : Resolves received data(string)
     */
    function read(int $length, bool $read_some = true, callable $callback, $argument = null) {}

    /**
     * Write asynchronously to stream descriptor.
     *
     * @param string $data : Write buffer
     * @param bool $write_some
     * @param callable $callback[optional] : Write handler callback
     * @param mixed $argument
     * @return Future : Resolves bytes transferred(int)
     */
    function write(string $data, bool $write_some = false, callable $callback, $argument = null) {}

    /**
     * Release ownership of the native descriptor implementation.
     *
     * @return void
     */
    function release() {}

    /**
     * {@inheritdoc}
     */
    function cancel() {}

    /**
     * {@inheritdoc}
     */
    function destroy() {}
}