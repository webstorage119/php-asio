<?php
/**
 * php-asio/stubs/Signal.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class Signal
 *
 * Provides signal handling functionality.
 *
 * @package Asio
 */
final class Signal
{
    /**
     * This class can only be instantiated using `Service::addSignal()`.
     */
    private function __construct() {}

    /**
     * Add the specified signal(s) to the signal set.
     *
     * @param int[] ...$signals[optional]
     * @return int : Error code
     */
    function add(int... $signals) {}

    /**
     * Initiate an asynchronous wait against the signal set.
     *
     * @param callable $callback[optional]
     * @param mixed $argument
     * @return Future
     */
    function wait(callable $callback, $argument = null) {}

    /**
     * Remove the specified signal(s) from the signal set.
     *
     * @param int[] ...$signals[optional]
     * @return int : Error code
     */
    function remove(int... $signals) {}

    /**
     * Remove all signals from the signal set.
     *
     * @return int : Error code
     */
    function clear() {}

    /**
     * Cancel current signal set.
     *
     * @return int : Error code
     */
    function cancel() {}
}