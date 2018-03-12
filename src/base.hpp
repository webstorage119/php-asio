/**
 * php-asio/base.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

namespace asio
{
    /// A base class for all I/O objects.
    class base
    {
    protected:
        /// IO service of this object.
        boost::asio::io_service& io_service_;

        /// Count of pending async handlers.
        unsigned short handler_count_ = 0;

        /// Constructor.
        explicit base(boost::asio::io_service& io_service) : io_service_(io_service) {}

    public:
        /// Deleted default constructor.
        base() = delete;

        /// Default destructor.
        ~base() = default;

        /// Deleted copy constructor.
        base(const base&) = delete;

        /// Deleted copy assignment operator.
        base& operator=(const base&) = delete;

        /// Increment handler count.
        unsigned short handler_count_inc()
        {
            return ++handler_count_;
        }

        /// Decrement handler count.
        unsigned short handler_count_dec()
        {
            return --handler_count_;
        }
    };
}
