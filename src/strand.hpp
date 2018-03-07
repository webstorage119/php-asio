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
    meth([cb, args]() { \
        INIT_RETVAL(); \
        call_user_function(CG(function_table), nullptr, cb, PASS_RETVAL, 1, args); \
        CORO_REGISTER(retval); \
        zval_ptr_dtor(cb); \
        zval_ptr_dtor(args); \
        efree(cb); \
        efree(args); \
    })

#ifdef ENABLE_STRAND
namespace Asio
{
    /// Wrapper for Boost.Asio strand.
    /// Provides serialised handler execution.
    class Strand : public Base
    {
        /// Boost.Asio strand instance.
        boost::asio::strand strand_;

    public:
        /// Constructor.
        explicit Strand(
            boost::asio::io_service& io_service
        ) : Base(io_service), strand_(io_service) {}

        /// Deleted default constructor.
        Strand() = delete;

        /// Deleted copy constructor.
        Strand(const Strand&) = delete;

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

        PHP_ASIO_CE_DECLARE();
    };
}
#endif // ENABLE_STRAND
