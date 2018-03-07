/**
 * php-asio/future.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "future.hpp"
#include "generator.hpp"
#include "zend_generators.h"

namespace Asio
{
    Future* Future::add(
#ifdef ENABLE_COROUTINE
        zend_object*& obj)
    {
        obj = p3::alloc_object<Future>(class_entry, [](Future* ptr)
        {
            new(ptr) Future;
        });
        GC_ADDREF(obj);
        return p3::to_object<Future>(obj);
#else
        ) {
        return new Future;
#endif // ENABLE_COROUTINE
    }

    template <typename T>
    void Future::on_resolve(const ASYNC_CALLBACK(T)&& callback)
    {
        callback_ = new ASYNC_CALLBACK(T)(std::move(callback));
    }

    template <typename T>
    void Future::resolve(const boost::system::error_code& ec, T arg)
    {
        auto callback = static_cast<ASYNC_CALLBACK(T)*>(callback_);
        send_ = (*callback)(ec, arg);
#ifdef ENABLE_COROUTINE
        if (yield_) {
            last_error_ = static_cast<int64_t>(ec.value());
            generator_send(reinterpret_cast<zend_generator*>(Z_OBJ_P(generator_)), send_);
            coroutine(generator_);
        }
#endif // ENABLE_COROUTINE
        zval_ptr_dtor(send_);
        efree(send_);
        delete callback;
#ifdef ENABLE_COROUTINE
        PHP_ASIO_OBJ_DTOR(this);
#else
        delete this;
#endif // ENABLE_COROUTINE
    }

#ifdef ENABLE_STRAND
    zval* Future::strand(zval* callable)
    {
        if (callable && Z_TYPE_P(callable) == IS_OBJECT &&
            instanceof_function(Z_OBJCE_P(callable), WrappedHandler::class_entry)) {
            auto wrapped_hander = p3::to_object<WrappedHandler>(callable);
            strand_ = &wrapped_hander->strand_;
            return wrapped_hander->callback_;
        }
        return callable;
    }
#endif // ENABLE_STRAND

#ifdef ENABLE_COROUTINE
    void Future::coroutine(zval* value)
    {
        if (Z_TYPE_P(value) == IS_OBJECT && instanceof_function(Z_OBJCE_P(value), zend_ce_generator)) {
            const auto generator = reinterpret_cast<zend_generator*>(Z_OBJ_P(value));
            if (generator_valid(generator)) {
                const auto ret = generator_current(generator);
                if (ret && instanceof_function(Z_OBJCE_P(ret), class_entry)) {
                    const auto future = p3::to_object<Future>(ret);
                    future->generator_ = value;
                    future->yield_ = true;
                    return;
                }
                PHP_ASIO_ERROR(E_WARNING, "Invalid yield value. Future expected.");
            }
        }
        zval_ptr_dtor(value);
        efree(value);
    }

    P3_METHOD(Future, lastError)
    {
        RETVAL_LONG(last_error_)
    }

    thread_local int64_t Future::last_error_ = 0;

    PHP_ASIO_CE_DEFINE(Future);
#endif // ENABLE_COROUTINE

    template void Future::on_resolve(const ASYNC_CALLBACK(int)&&);
    template void Future::on_resolve(const ASYNC_CALLBACK(size_t)&&);
    template void Future::on_resolve(const ASYNC_CALLBACK(tcp::resolver::iterator)&&);
    template void Future::on_resolve(const ASYNC_CALLBACK(udp::resolver::iterator)&&);
    template void Future::resolve(const boost::system::error_code&, int);
    template void Future::resolve(const boost::system::error_code&, size_t);
    template void Future::resolve(const boost::system::error_code&, tcp::resolver::iterator);
    template void Future::resolve(const boost::system::error_code&, udp::resolver::iterator);
}
