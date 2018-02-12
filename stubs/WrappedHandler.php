<?php
/**
 * php-asio/stubs/WrappedHandler.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class WrappedHandler
 *
 * Async handler callback wrapped by strand.
 *
 * @package Asio
 */
class WrappedHandler
{
    /**
     * This class can only be instantiated using `Strand::wrap()`.
     */
    private function __construct() {}

    /**
     * Making object callable.
     *
     * Note: Calling directly has no effect.
     */
    function __invoke() {}
}