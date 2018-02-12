<?php
/**
 * php-asio/stubs/Socket.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface Socket
 *
 * @package Asio
 */
interface Socket
{
    /**
     * Determine the number of bytes available for reading.
     *
     * @param int $ec[optional] : Error code
     * @return int
     */
    function available(int &$ec);

    /**
     * Determine whether the socket is at the out-of-band data mark.
     *
     * @param int $ec[optional] : Error code
     * @return bool
     */
    function atMark(int &$ec);

    /**
     * Close socket.
     *
     * @return int : Error code
     */
    function close();
}