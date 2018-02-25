/**
 * php-asio/strand.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "strand.hpp"
#include "future.hpp"

#ifdef ENABLE_STRAND
namespace Asio
{
    P3_METHOD(Strand, dispatch)
    {
        PHP_ASIO_DISPATCH_CALLBACK(strand_.dispatch);
    }

    P3_METHOD(Strand, post)
    {
        PHP_ASIO_DISPATCH_CALLBACK(strand_.post);
    }

    P3_METHOD(Strand, runningInThisThread) const
    {
        RETVAL_BOOL(strand_.running_in_this_thread());
    }

    P3_METHOD(Strand, wrap)
    {
        zval* callable = nullptr;
        ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ZVAL(callable)
        ZEND_PARSE_PARAMETERS_END();
        auto obj = p3::allocObject<WrappedHandler>(
            WrappedHandler::class_entry, [this, callable](WrappedHandler* ptr) {
                new(ptr) WrappedHandler(strand_, callable);
            });
        GC_ADDREF(obj);
        RETVAL_OBJ(obj);
    }

    P3_METHOD(Strand, destroy)
    {
        PHP_ASIO_OBJ_DTOR();
    }

    PHP_ASIO_CE_DEFINE(Strand);
}
#endif // ENABLE_STRAND
