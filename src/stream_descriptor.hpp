/**
 * php-asio/stream_descriptor.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

namespace Asio
{
    /// Wrapper for Boost.Asio stream descriptor.
    class StreamDescriptor : public Base
    {
        /// Boost.Asio stream descriptor instance.
        boost::asio::posix::stream_descriptor stream_descriptor_;

        /// Read handler for stream descriptor.
        zval* read_handler(const boost::system::error_code& error,
            size_t length, zend_string* buffer, zval* callback, zval* argument);

        /// Write handler for socket.
        zval* write_handler(const boost::system::error_code& error,
            size_t length, zend_string* buffer, zval* callback, zval* argument);

    public:
        /// Constructor.
        explicit StreamDescriptor(
            boost::asio::io_service& io_service
        ) : Base(io_service), stream_descriptor_(io_service) {}

        /* {{{ proto int StreamDescriptor::bind(int|resource fd);
         * Opens the descriptor to hold an existing native descriptor. */
        P3_METHOD_DECLARE(assign);
        /* }}} */

        /* {{{ proto bool StreamDescriptor::isOpen(void);
         * Determine whether the descriptor is open. */
        P3_METHOD_DECLARE(isOpen);
        /* }}} */

        /* {{{ proto Future StreamDescriptor::read(int length, [bool read_some = true],
         *               [callable callback], [mixed argument]);
         * Read asynchronously from stream descriptor. */
        P3_METHOD_DECLARE(read);
        /* }}} */

        /* {{{ proto Future StreamDescriptor::write(string data, [bool write_some = false],
         *               [callable callback], [mixed argument]);
         * Write asynchronously to stream socket. */
        P3_METHOD_DECLARE(write);
        /* }}} */

        /* {{{ proto void StreamDescriptor::release(void);
         * Release ownership of the native descriptor implementation. */
        P3_METHOD_DECLARE(release);
        /* }}} */

        /* {{{ proto int StreamDescriptor::cancel(void);
         * Cancel all asynchronous operations on the descriptor. */
        P3_METHOD_DECLARE(cancel);
        /* }}} */

        /* {{{ proto int StreamDescriptor::close(void);
         * Close the descriptor. */
        P3_METHOD_DECLARE(close);
        /* }}} */

        PHP_ASIO_CE_DECLARE();
    };
}
