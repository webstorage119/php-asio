<?php
/**
 * php-asio/stubs/IoObject.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface IoObject
 *
 * Base class of all I/O objects.
 *
 * @package Asio
 */
interface IoObject
{
    /**
     * Cancel all asynchronous operations on this I/O object.
     *
     * @return int : Error code
     */
    function cancel();

    /**
     * Destroy this I/O object.
     *
     * @return void
     */
    function destroy();
}