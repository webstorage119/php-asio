/**
 * php-asio/signal.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

namespace Asio
{
    /// Wrapper for Boost.Asio signal_set.
    /// Provides functionalities for signal handling.
    class Signal : public Base
    {
        /// Boost.Asio signal_set instance.
        boost::asio::signal_set signal_;

        /// Signal handler callback.
        zval* handler(const boost::system::error_code& error,
            int signal, zval* callback, zval* argument);

    public:
        /// Constructor.
        explicit Signal(
            boost::asio::io_service& io_service
        ) : Base(io_service), signal_(io_service) {}

        /* {{{ proto int Signal::add(int sig_num, [int ...]);
         * Add the specified signal(s) to the signal set. */
        P3_METHOD_DECLARE(add);
        /* }}} */

        /* {{{ proto int Signal::remove(int sig_num, [int ...]);
         * Remove the specified signal(s) from the signal set. */
        P3_METHOD_DECLARE(remove);
        /* }}} */

        /* {{{ proto Future Signal::wait([callable callback], [mixed argument]);
         * Initiate an asynchronous wait against the signal set. */
        P3_METHOD_DECLARE(wait);
        /* }}} */

        /* {{{ proto int Signal::clear(void);
         * Remove all signals from the signal set. */
        P3_METHOD_DECLARE(clear);
        /* }}} */

        /* {{{ proto int Signal::cancel(void);
         * Cancel current signal set. */
        P3_METHOD_DECLARE(cancel);
        /* }}} */

        PHP_ASIO_CE_DECLARE();
    };
}
