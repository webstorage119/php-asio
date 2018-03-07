/**
 * php-asio/resolver.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

namespace Asio
{
    /// Wrapper for Boost.Asio resolver.
    /// Provides hostname resolution.
    template <typename Protocol>
    class Resolver : public Base
    {
        /// Resolver iterator which holds an endpoint.
        using iterator = typename Protocol::resolver::iterator;

        /// Boost.Asio resolver instance.
        typename Protocol::resolver resolver_;

        /// Resolve handler.
        zval* handler(const boost::system::error_code& error,
            iterator iter, zval* callback, zval* argument);

    public:
        /// Constructor.
        explicit Resolver(
            boost::asio::io_service& io_service
        ) : Base(io_service), resolver_(io_service) {}

        /* {{{ proto Future Resolver::resolve(string host, [string service = ""],
         *               [callable callback], [mixed argument]);
         * Initiate an asynchronous resolve against the resolver. */
        P3_METHOD_DECLARE(resolve);
        /* }}} */

        /* {{{ proto int Resolver::cancel(void);
         * Cancel all asynchronous operations on this resolver. */
        P3_METHOD_DECLARE(cancel);
        /* }}} */

        PHP_ASIO_CE_DECLARE();
    };

    using TcpResolver = Resolver<tcp>;
    using UdpResolver = Resolver<udp>;
}
