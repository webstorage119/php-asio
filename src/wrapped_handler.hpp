/**
 * php-asio/wrapped_handler.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "strand.hpp"

#ifdef ENABLE_STRAND
namespace asio
{
    /// Async handler callback wrapped by strand.
    class wrapped_handler
    {
        /// Strand object for this wrapped handler.
        strand* strand_;

        /// Wrapped handler callback.
        zval* callback_;

        // Future has access to Strand and handler callback.
        friend class future;
        
    public:
        /// Constructor.
        explicit wrapped_handler(
            strand* strand, zval* callback
        ) : strand_(strand), callback_(callback)
        {
            GC_ADDREF(Z_COUNTED_P(callback_));
        }

        /// Deleted default constructor.
        wrapped_handler() = delete;

        /// Deleted copy constructor.
        wrapped_handler(const wrapped_handler&) = delete;

        /// Make this object callable.
        P3_METHOD_DECLARE(__invoke) {}

        PHP_ASIO_CE_DECLARE();
    };
}
#endif // ENABLE_STRAND
