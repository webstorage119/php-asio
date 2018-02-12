<?php
/**
 * php-asio/stubs/UdpResolver.php
 *
 * This file is a stub of php-asio, which is implemented in extension asio.so.
 * For source code of asio.so, see src/ directory.
 *
 * @author CismonX<admin@cismon.net>
 */

namespace Asio;

/**
 * Class UdpResolver
 *
 * Provides the ability to resolve a query to an array of endpoints.
 * Remote service should use UDP as transport protocol.
 *
 * @package Asio
 */
final class UdpResolver implements Resolver
{
    /**
     * This class can only be instantiated using `Service::addTcpResolver()`.
     */
    private function __construct() {}

    /**
     * {@inheritdoc}
     */
    function resolve(string $host, string $service = '', callable $callback = null, $argument = null) {}

    /**
     * {@inheritdoc}
     */
    function cancel() {}
}