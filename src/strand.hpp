/**
 * php-asio/strand.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "base.hpp"

#define PHP_ASIO_DISPATCH_CALLBACK(meth) \
    zval* callback; \
    zval* argument = nullptr; \
    ZEND_PARSE_PARAMETERS_START(1, 2) \
        Z_PARAM_ZVAL(callback) \
        Z_PARAM_OPTIONAL \
        Z_PARAM_ZVAL(argument) \
    ZEND_PARSE_PARAMETERS_END(); \
    ZVAL_PTR_INIT(cb); \
    ZVAL_COPY(cb, callback); \
    ZVAL_PTR_INIT(args); \
    if (argument) \
        ZVAL_COPY(args, argument); \
    else \
        ZVAL_NULL(args); \
    meth([this, cb, args]() { \
        INIT_RETVAL(); \
        call_user_function(CG(function_table), nullptr, cb, PASS_RETVAL, 1, args); \
        CORO_REGISTER(retval); \
        zval_ptr_dtor(cb); \
        zval_ptr_dtor(args); \
        efree(cb); \
        efree(args); \

#ifdef ENABLE_STRAND
namespace asio
{
    /// Wrapper for Boost.Asio strand.
    /// Provides serialized handler execution.
    class strand : public base
    {
        /// Boost.Asio strand instance.
        boost::asio::strand strand_;

    public:
        /// Constructor.
        explicit strand(
            boost::asio::io_service& io_service
        ) : base(io_service), strand_(io_service) {}

        /// Deleted default constructor.
        strand() = delete;

        /// Deleted copy constructor.
        strand(const strand&) = delete;

        /* {{{ proto void Strand::dispatch(callable callback, [mixed argument]);
         * Request the strand to invoke the given handler. */
        P3_METHOD_DECLARE(dispatch);
        /* }}} */

        /* {{{ proto void Strand::post(callable callback, [mixed argument]);
         * Request the strand to invoke the given handler. */
        P3_METHOD_DECLARE(post);
        /* }}} */

        /* {{{ proto bool Strand::runningInThisThread(void);
         * Determine whether the strand is running in the current thread. */
        P3_METHOD_DECLARE(runningInThisThread) const;
        /* }}} */

        /* {{{ proto WrappedHandler wrap([callable callback]);
         * Create a new handler that automatically dispatches the wrapped handler on the strand. */
        P3_METHOD_DECLARE(wrap);
        /* }}} */

        /// Returns the wrapped Boost.Asio strand.
        boost::asio::strand* implmentation()
        {
            return &strand_;
        }

        PHP_ASIO_CE_DECLARE();
    };
}
#endif // ENABLE_STRAND
