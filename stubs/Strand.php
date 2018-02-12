<?php
/**
 * php-asio/stubs/Strand.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class Strand
 *
 * Provides serialised handler execution.
 *
 * @package Asio
 */
final class Strand
{
    /**
     * This class can only be instantiated using `Service::addStrand()`.
     */
    private function __construct() {}

    /**
     * Request the strand to invoke the given handler.
     *
     * @param callable $callback
     * @param mixed $argument
     * @return void
     */
    function dispatch(callable $callback, $argument = null) {}

    /**
     * Request the strand to invoke the given handler and return immediately.
     *
     * @param callable $callback
     * @param mixed $argument
     * @return void
     */
    function post(callable $callback, $argument = null) {}

    /**
     * Determine whether the strand is running in the current thread.
     *
     * @return bool
     */
    function runningInThisThread() {}

    /**
     * Create a new handler that automatically dispatches the wrapped handler on the strand.
     *
     * @param callable $callback[optional]
     * @return WrappedHandler
     */
    function wrap(callable $callback) {}
}