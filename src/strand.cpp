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
        RETVAL_OBJ(p3::alloc_object<WrappedHandler>(WrappedHandler::class_entry,
            [this, callable](WrappedHandler* ptr) {
                new(ptr) WrappedHandler(strand_, callable);
            }));
    }

    PHP_ASIO_CE_DEFINE(Strand);
}
#endif // ENABLE_STRAND
