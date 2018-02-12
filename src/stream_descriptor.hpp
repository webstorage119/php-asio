/**
 * php-asio/include/stream_descriptor.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

#define PHP_ASIO_INET_ASSIGN(obj, p) \
    zend_bool inet6; \
    zval* fd; \
    ZEND_PARSE_PARAMETERS_START(2, 2) \
        Z_PARAM_BOOL(inet6) \
        Z_PARAM_ZVAL(fd) \
    ZEND_PARSE_PARAMETERS_END(); \
    boost::system::error_code ec; \
    const auto protocol = inet6 ? p::v6() : p::v4(); \
    if (UNEXPECTED(Z_TYPE_P(fd) == IS_LONG)) \
        obj.assign(protocol, Z_LVAL_P(fd), ec); \
    else \
        obj.assign(protocol, StreamDescriptor::resource_to_fd(fd), ec); \
    RETVAL_EC(ec)

#define PHP_ASIO_LOCAL_ASSIGN(obj, p) \
    zval* fd; \
    ZEND_PARSE_PARAMETERS_START(1, 1) \
        Z_PARAM_ZVAL(fd) \
    ZEND_PARSE_PARAMETERS_END(); \
    boost::system::error_code ec; \
    if (UNEXPECTED(Z_TYPE_P(fd) == IS_LONG)) \
        obj.assign(p(), Z_LVAL_P(fd), ec); \
    else \
        obj.assign(p(), StreamDescriptor::resource_to_fd(fd), ec); \
    RETVAL_EC(ec)

namespace Asio
{
    class StreamDescriptor : public Base
    {
        /// Boost.Asio stream descriptor instance.
        boost::asio::posix::stream_descriptor stream_descriptor_;

    public:
        /**
         * Constructor.
         * @param io_service : I/O service of current stream_descriptor.
         */
        explicit StreamDescriptor(
            boost::asio::io_service& io_service
        ) : Base(io_service), stream_descriptor_(io_service) {}

        /**
         * Extract a valid poll fd from a PHP resource.
         * @param resource : Resource containing a valid file descriptor
         */
        static int resource_to_fd(zval* resource);

        PHP_ASIO_CE_DECLARE();
    };
}
