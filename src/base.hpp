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
    template <typename T>
    class base
    {
    protected:
        /// IO service of this object.
        boost::asio::io_service& io_service_;

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

        /* {{{ proto void IoObject::destroy(void);
         * Destroy this I/O object. */
        P3_METHOD_DECLARE(destroy)
        {
            PHP_ASIO_OBJ_DTOR(static_cast<T*>(this));
        }
        /* }}} */
    };
}
