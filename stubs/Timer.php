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
     * Set the timer's expiry time relative to now.
     *
     * @param int $duration : Milliseconds from now.
     * @return int : Error code.
     */
    function expiresFromNow(int $duration) {}

    /**
     * Set the timer's expiry time as an absolute time.
     *
     * @param int $timestamp : UNIX timestamp(in milliseconds).
     * @return int : Error code.
     */
    function expiresAt(int $timestamp) {}

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
}