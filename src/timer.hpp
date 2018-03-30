/**
 * php-asio/timer.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

namespace asio
{
    /// Wrapper for Boost.Asio deadline timer.
    class timer : public base
    {
        /// Boost.Asio timer instance.
        boost::asio::deadline_timer timer_;

        /// Handler for timer callback.
        zval* handler(const boost::system::error_code& error,
            zval* callback, zval* argument);

    public:
        /// Constructor.
        explicit timer(
            boost::asio::io_service& io_service
        ) : base(io_service), timer_(io_service) {}

        /* {{{ proto int Timer::expiresFromNow(int duration);
         * Set the timer's expiry time relative to now. */
        P3_METHOD_DECLARE(expiresFromNow);
        /* }}} */

        /* {{{ proto int Timer::expiresFromNow(int timestamp);
        * Set the timer's expiry time as an absolute time. */
        P3_METHOD_DECLARE(expiresAt);
        /* }}} */

        /* {{{ proto Future Timer::wait([callable callback], [mixed argument]);
         * Initiate an asynchronous wait against the timer. */
        P3_METHOD_DECLARE(wait);
        /* }}} */

        /* {{{ proto int Timer::cancel(void);
         * Cancel timer. */
        P3_METHOD_DECLARE(cancel);
        /* }}} */

        PHP_ASIO_CE_DECLARE();
    };
}