/**
 * php-asio/strand.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "strand.hpp"
#include "future.hpp"

#ifdef ENABLE_STRAND
namespace asio
{
    P3_METHOD(strand, dispatch)
    {
        PHP_ASIO_DISPATCH_CALLBACK(strand_.dispatch);
    }

    P3_METHOD(strand, post)
    {
        PHP_ASIO_DISPATCH_CALLBACK(strand_.post);
    }

    P3_METHOD(strand, runningInThisThread) const
    {
        RETVAL_BOOL(strand_.running_in_this_thread());
    }

    P3_METHOD(strand, wrap)
    {
        zval* callable = nullptr;
        ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ZVAL(callable)
        ZEND_PARSE_PARAMETERS_END();
        RETVAL_OBJ(p3::alloc_object<wrapped_handler>(wrapped_handler::class_entry,
            [this, callable](wrapped_handler* ptr) {
                new(ptr) wrapped_handler(strand_, callable);
            }));
    }

    PHP_ASIO_CE_DEFINE(strand);
}
#endif // ENABLE_STRAND
