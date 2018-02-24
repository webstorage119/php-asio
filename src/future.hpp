/**
 * php-asio/include/future.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"
#include "wrapped_handler.hpp"

#define ASYNC_CALLBACK(type) std::function<zval*(const boost::system::error_code&, type)>

namespace Asio
{
    /**
     * Class Future.
     * When an asynchronous operation completes, its Future will be resolved.
     * And the corresponding coroutine will resume (if Future was yielded by a Generator).
     */
    class Future
    {
        /// Handler callback of the async operation.
        void* callback_ = nullptr;

#ifdef ENABLE_COROUTINE
        /// Last error code emitted by yielded async operations of this thread.
        static thread_local int64_t last_error_;

        /// Generator instance which yielded this Future.
        zval* generator_ = nullptr;

        /// Whether this future is yielded by a Generator.
        bool yield_ = false;
#endif // ENABLE_COROUTINE

        /// Value which will be sent back to the Generator.
        zval* send_ = nullptr;

#ifdef ENABLE_STRAND
        /// Pointer to Strand which wrapped this Future.
        boost::asio::strand* strand_ = nullptr;
#endif // ENABLE_STRAND

        /**
         * Make default constructor private to avoid user instantiation.
         */
        explicit Future() = default;

    public:
        /**
         * Create a new Future instance.
         */
        static Future* add(
#ifdef ENABLE_COROUTINE
            zend_object*& obj
#endif // ENABLE_COROUTINE
        );

        /**
         * Deleted copy constructor.
         */
        Future(const Future&) = delete;

        /**
         * Deleted copy assignment operator.
         */
        Future& operator=(const Future&) = delete;

        /**
         * Set future resolver callback.
         */
        template <typename T>
        void on_resolve(const ASYNC_CALLBACK(T)&& callback);

        /**
         * Resolve the Future upon operation completion.
         * @param ec : Error code
         * @param arg : Dependent argument
         */
        template <typename T>
        void resolve(const boost::system::error_code& ec, T arg);

#ifdef ENABLE_STRAND
        zval* strand(zval* callable);

        /**
         * Get the pointer to the strand which wrapped this Future.
         */
        boost::asio::strand* get_strand() const
        {
            return strand_;
        }
#endif // ENABLE_STRAND

#ifdef ENABLE_COROUTINE
        /**
         * Attempt to start/resume a coroutine with a PHP Generator.
         */
        static void coroutine(zval* generator);

        /**
         * Get last error emitted by handler callback within yielded Future.
         */
        static P3_METHOD_DECLARE(lastError);

        PHP_ASIO_CE_DECLARE();
#endif // ENABLE_COROUTINE
    };
}
