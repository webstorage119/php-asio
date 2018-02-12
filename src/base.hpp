/**
 * php-asio/include/base.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

/**
 * A base class for all I/O objects.
 */
namespace Asio
{
    class Base
    {
    protected:
        /// IO service of this object.
        boost::asio::io_service& io_service_;

        /**
         * Constructor.
         * @param io_service : I/O service for current object
         */
        explicit Base(boost::asio::io_service& io_service) : io_service_(io_service) {}

        /**
         * Deleted default constructor.
         */
        Base() = delete;

        /**
         * Deleted copy constructor.
         */
        Base(const Base&) = delete;

        /**
         * Deleted copy assignment operator.
         */
        Base& operator=(const Base&) = delete;
    };
}
