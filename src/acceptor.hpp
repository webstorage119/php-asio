/**
 * php-asio/acceptor.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"
#include "socket.hpp"

namespace Asio
{
    /// Wrapper for Boost.Asio stream socket acceptor.
    /// Provide TCP services.
    template <typename Protocol>
    class Acceptor : public Base
    {
        /// Boost.Asio acceptor instance.
        typename Protocol::acceptor acceptor_;

        /// Accept handler.
        zval* handler(const boost::system::error_code& error,
            Socket<Protocol>* socket, zval* callback, zval* argument);

    public:
        /// Constructor.
        explicit Acceptor(
            boost::asio::io_service& io_service
        ) : Base(io_service), acceptor_(io_service) {}

        P3_METHOD_DECLARE(open);

        P3_METHOD_DECLARE(assign);

        P3_METHOD_DECLARE(bind);

        P3_METHOD_DECLARE(listen);

        P3_METHOD_DECLARE(accept);

        P3_METHOD_DECLARE(cancel);

        P3_METHOD_DECLARE(close);

        PHP_ASIO_CE_DECLARE();
    };

    using TcpAcceptor = Acceptor<tcp>;
    using UnixAcceptor = Acceptor<unix>;
}
