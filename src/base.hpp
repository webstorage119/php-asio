/**
 * php-asio/base.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

/// A base class for all I/O objects.
namespace Asio
{
    class Base
    {
    protected:
        /// IO service of this object.
        boost::asio::io_service& io_service_;

        /// Constructor.
        explicit Base(boost::asio::io_service& io_service) : io_service_(io_service) {}

    public:
        /// Deleted default constructor.
        Base() = delete;

        /// Default destructor.
        virtual ~Base() = default;

        /// Deleted copy constructor.
        Base(const Base&) = delete;

        /// Deleted copy assignment operator.
        Base& operator=(const Base&) = delete;

        /* {{{ proto void IoObject::destroy();
         * Destroy this I/O object. */
        template <typename T>
        P3_METHOD_DECLARE(destroy)
        {
            PHP_ASIO_OBJ_DTOR(dynamic_cast<T*>(this));
        }
        /* }}} */
    };
}
