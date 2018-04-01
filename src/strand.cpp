/**
 * php-asio/strand.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "strand.hpp"
#include "future.hpp"

#ifdef ENABLE_STRAND

#define STRAND_DISPATCH_CALLBACK(meth) \
    PHP_ASIO_DISPATCH_CALLBACK(meth) PHP_ASIO_DEC_HANDLER_COUNT(); })

namespace asio
{
    P3_METHOD(strand, dispatch)
    {
        PHP_ASIO_INC_HANDLER_COUNT();
        STRAND_DISPATCH_CALLBACK(strand_.dispatch);
    }

    P3_METHOD(strand, post)
    {
        PHP_ASIO_INC_HANDLER_COUNT();
        STRAND_DISPATCH_CALLBACK(strand_.post);
    }

    P3_METHOD(strand, runningInThisThread) const
    {
        RETVAL_BOOL(strand_.running_in_this_thread());
    }

    P3_METHOD(strand, wrap)
    {
        zval* callback;
        ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ZVAL(callback)
        ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_INC_HANDLER_COUNT();
        RETVAL_OBJ(p3::alloc_object<wrapped_handler>(wrapped_handler::class_entry,
            [this, callback](wrapped_handler* ptr) {
                new(ptr) wrapped_handler(this, callback);
            }));
    }

    PHP_ASIO_CE_DEFINE(strand);
}

#endif // ENABLE_STRAND
