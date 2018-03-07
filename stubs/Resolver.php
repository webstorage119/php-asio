<?php
/**
 * php-asio/stubs/Resolver.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Interface Resolver
 *
 * @package Asio
 */
interface Resolver extends IoObject
{
    /**
     * Initiate an asynchronous resolve against the resolver.
     *
     * @param string $host : Host name
     * @param string $service
     * @param callable $callback[optional] : Resolver callback
     * @param mixed $argument
     * @return Future
     */
    function resolve(string $host, string $service = '', callable $callback, $argument = null);
}