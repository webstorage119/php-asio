/**
 * php-asio/include/timer.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

namespace Asio
{
    /**
     * Wrapper for Boost.Asio deadline timer.
     */
    class Timer : public Base
    {
        /// Boost.Asio timer instance.
        boost::asio::deadline_timer timer_;

        /**
         * Handler for timer callback.
         * @param error : error code
         */
        zval* handler(const boost::system::error_code& error,
            zval* callback, zval* argument);

    public:
        /**
         * Timer constructor.
         * @param io_service : I/O service for current timer
         */
        explicit Timer(
            boost::asio::io_service& io_service
        ) : Base(io_service), timer_(io_service) {}

        /* {{{ proto int Timer::expire(int time, [bool use_timestamp = false]);
         * Set timer expiry time. */
        P3_METHOD_DECLARE(expire);
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