/**
 * php-asio/acceptor.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"
#include "socket.hpp"

namespace asio
{
    /// Wrapper for Boost.Asio stream socket acceptor.
    /// Provide TCP services.
    template <typename Protocol>
    class acceptor : public base
    {
        /// Boost.Asio acceptor instance.
        typename Protocol::acceptor acceptor_;

        /// Accept handler.
        zval* handler(const boost::system::error_code& error,
            socket<Protocol>* socket, zval* callback, zval* argument);

    public:
        /// Constructor.
        explicit acceptor(
            boost::asio::io_service& io_service
        ) : base(io_service), acceptor_(io_service) {}

        P3_METHOD_DECLARE(open);

        P3_METHOD_DECLARE(assign);

        P3_METHOD_DECLARE(bind);

        P3_METHOD_DECLARE(listen);

        P3_METHOD_DECLARE(accept);

        P3_METHOD_DECLARE(cancel);

        P3_METHOD_DECLARE(close);

        PHP_ASIO_CE_DECLARE();
    };

    using tcp_acceptor = acceptor<tcp>;
    using unix_acceptor = acceptor<unix>;
}
