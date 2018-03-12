/**
 * php-asio/common.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "p3.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>

// Compatible with PHP 7.3
#if PHP_VERSION_ID < 70300
#define GC_ADDREF(p) ++GC_REFCOUNT(p)
#define GC_DELREF(p) --GC_REFCOUNT(p)
#endif

#if PHP_VERSION_ID < 70115 || (PHP_VERSION_ID > 70200 && PHP_VERSION_ID < 70203)

#if PHP_VERSION_ID < 70100
#define _zend_wrong_parameters_count_error(throw, ...) zend_wrong_paramers_count_error(__VA_ARGS__)
#elif PHP_VERSION_ID < 70200
#define _zend_wrong_parameters_count_error(throw, ...) zend_wrong_parameters_count_error(__VA_ARGS__)
#else
#define _zend_wrong_parameters_count_error(...) zend_wrong_parameters_count_error(__VA_ARGS__)
#endif

// See https://externals.io/message/101364 for details.
#undef ZEND_PARSE_PARAMETERS_START_EX
#define ZEND_PARSE_PARAMETERS_START_EX(flags, min_num_args, max_num_args) do { \
    const int _flags = (flags); \
    int _min_num_args = (min_num_args); \
    int _max_num_args = (max_num_args); \
    int _num_args = EX_NUM_ARGS(); \
    int _i; \
    zval *_real_arg, *_arg = NULL; \
    zend_expected_type _expected_type = Z_EXPECTED_LONG; \
    char *_error = NULL; \
    zend_bool _dummy; \
    zend_bool _optional = 0; \
    int error_code = ZPP_ERROR_OK; \
    ((void)_i); \
    ((void)_real_arg); \
    ((void)_arg); \
    ((void)_expected_type); \
    ((void)_error); \
    ((void)_dummy); \
    ((void)_optional); \
    do { \
        if (UNEXPECTED(_num_args < _min_num_args) || \
            (UNEXPECTED(_num_args > _max_num_args) && \
             EXPECTED(_max_num_args >= 0))) { \
            if (!(_flags & ZEND_PARSE_PARAMS_QUIET)) { \
                _zend_wrong_parameters_count_error(_flags & ZEND_PARSE_PARAMS_THROW, \
                    _num_args, _min_num_args, _max_num_args); \
            } \
            error_code = ZPP_ERROR_FAILURE; \
            break; \
        } \
        _i = 0; \
        _real_arg = ZEND_CALL_ARG(execute_data, 0);

#endif

#define PHP_ASIO_OBJ_ALLOC(obj, type, arg) \
    auto obj = p3::alloc_object<type>(type::class_entry, [this](type* ptr) { \
        new(ptr) type(arg); \
    })

#define PHP_ASIO_OBJ_DTOR(obj) GC_DELREF(p3::to_zend_object(obj))

#define ZVAL_ALLOC(name) name = static_cast<zval*>(emalloc(sizeof(zval)))
#define ZVAL_PTR_INIT(name) auto ZVAL_ALLOC(name)
#define ZVAL_INIT(name) zval name = {{ 0 }}

#define RETVAL_EC(ec) RETVAL_LONG(static_cast<zend_long>((ec).value()))

#define PHP_ASIO_ERROR(type, msg) php_error_docref(nullptr, type, msg)

#define PHP_ASIO_CE_DECLARE() \
    static zend_class_entry* class_entry; \
    static zend_object_handlers handlers
#define PHP_ASIO_CE_DEFINE(type) \
    zend_class_entry* type::class_entry; \
    zend_object_handlers type::handlers

 // Handlers with one argument is treated as ones with two arguments.
#define NOARG int
#define ASYNC_HANDLER_SINGLE_ARG \
    std::function<void(const boost::system::error_code&)>(boost::bind( \
        &future::resolve<std::remove_pointer_t<decltype(this)>, NOARG>, \
        future, boost::asio::placeholders::error, 0))
#define ASYNC_HANDLER_DOUBLE_ARG(obj_type) \
    std::function<void(const boost::system::error_code&, obj_type)>(boost::bind( \
        &future::resolve<std::remove_pointer_t<decltype(this)>, obj_type>, \
        future, boost::asio::placeholders::error, _2))

// If you don't need coroutines, you can turn it off for better performance.
#ifdef ENABLE_COROUTINE
#define CORO_REGISTER(value) future::coroutine(value)
#define FUTURE_INIT() \
    zend_object* obj; \
    auto future = future::add(this, obj);
#define FUTURE_RETURN() RETVAL_OBJ(obj)
#define INIT_RETVAL() \
    ZVAL_PTR_INIT(retval); \
    ZVAL_NULL(retval)
#define PASS_RETVAL retval
#else
#define CORO_REGISTER(value)
#define FUTURE_INIT() auto future = future::add(this)
#define FUTURE_RETURN()
#define INIT_RETVAL() ZVAL_INIT(retval)
#define PASS_RETVAL &retval
#endif // ENABLE_COROUTINE

#define CORO_RETURN_NULL() \
    ZVAL_PTR_INIT(retval); \
    ZVAL_NULL(retval); \
    return retval
#define CORO_RETURN(type, value) \
    ZVAL_PTR_INIT(retval); \
    type(retval, value); \
    return retval

// If you don't need multi-threading support for I/O objects, you can disable Strand for better performance.
#ifdef ENABLE_STRAND
#define STRAND_UNWRAP() future->strand(cb)
#define STRAND_RESOLVE(arg) future->get_strand() ? future->get_strand()->wrap(arg) : arg
#else
#define STRAND_UNWRAP() cb
#define STRAND_RESOLVE(arg) arg
#endif // ENABLE_STRAND

#define PHP_ASIO_INVOKE_CALLBACK_START(argc) \
    const auto _argc = argc; \
    if (callback && zend_is_callable(callback, 0, nullptr)) { \
        zval arguments[_argc] = {{{ 0 }}}; \
        ZVAL_OBJ(&arguments[0], p3::to_zend_object(this));

#define PHP_ASIO_INVOKE_CALLBACK() \
        ZVAL_LONG(&arguments[_argc - 2], static_cast<zend_long>(error.value())); \
        if (argument) \
            ZVAL_COPY(&arguments[_argc - 1], argument); \
        else \
            ZVAL_NULL(&arguments[_argc - 1]); \
        INIT_RETVAL(); \
        call_user_function(CG(function_table), nullptr, callback, PASS_RETVAL, _argc, arguments)

#define PHP_ASIO_INVOKE_CALLBACK_END() \
        CORO_REGISTER(retval); \
        zval_ptr_dtor(callback); \
        efree(callback); \
    } \
    if (argument) { \
        zval_ptr_dtor(argument); \
        efree(argument); \
    }

// To ensure the callback and the extra arg is still alive when async operation resolves,
// We shall allocate new memory on the heap.
#define PHP_ASIO_FUTURE_INIT() \
    if (handler_count_inc() == 1) \
        GC_ADDREF(p3::to_zend_object(this)); \
    zval* cb = nullptr; \
    if (callback) { \
        ZVAL_ALLOC(cb); \
        ZVAL_COPY(cb, callback); \
    } \
    zval* args = nullptr; \
    if (argument) { \
        ZVAL_ALLOC(args); \
        ZVAL_COPY(args, argument); \
    } \
    FUTURE_INIT()

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using unix = boost::asio::local::stream_protocol;
using udg = boost::asio::local::datagram_protocol;
