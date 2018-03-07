<?php
/**
 * php-asio/stubs/Timer.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Wrapper for Boost.Asio deadline_timer.
 *
 * @package Asio
 */
final class Timer implements IoObject
{
    /**
     * This class can only be instantiated using "Service::addTimer()".
     */
    private function __construct() {}

    /**
     * Set timer expiry time.
     *
     * @param int $time : Milliseconds from now or UNIX timestamp(in milliseconds).
     * @param bool $use_timestamp : Use timestamp or relative time as first parameter.
     * @return int : Error code.
     */
    function expire(int $time, bool $use_timestamp = false) {}

    /**
     * Initiate an asynchronous wait against the timer.
     *
     * @param callable $callback[optional]
     * @param mixed $argument
     * @return Future : Resolves null
     */
    function wait(callable $callback, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function cancel() {}

    /**
     * {@inheritdoc}
     */
    function destroy() {}
}