/**
 * php-asio/io.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

#define PHP_ASIO_INET_ASSIGN(obj, p) \
    zend_bool inet6; \
    zval* fd; \
    ZEND_PARSE_PARAMETERS_START(2, 2) \
        Z_PARAM_BOOL(inet6) \
        Z_PARAM_ZVAL(fd) \
    ZEND_PARSE_PARAMETERS_END(); \
    boost::system::error_code ec; \
    const auto protocol = inet6 ? p::v6() : p::v4(); \
    if (UNEXPECTED(Z_TYPE_P(fd) == IS_LONG)) \
        (obj).assign(protocol, Z_LVAL_P(fd), ec); \
    else \
        (obj).assign(protocol, resource_to_fd(fd), ec); \
    RETVAL_EC(ec)

#define PHP_ASIO_LOCAL_ASSIGN(obj, p) \
    zval* fd; \
    ZEND_PARSE_PARAMETERS_START(1, 1) \
        Z_PARAM_ZVAL(fd) \
    ZEND_PARSE_PARAMETERS_END(); \
    boost::system::error_code ec; \
    if (UNEXPECTED(Z_TYPE_P(fd) == IS_LONG)) \
        (obj).assign(p(), Z_LVAL_P(fd), ec); \
    else \
        (obj).assign(p(), resource_to_fd(fd), ec); \
    RETVAL_EC(ec)

#if defined(ENABLE_NULL_BUFFERS) && BOOST_VERSION >= 106600
 // Null buffers are deprecated as of Boost 1.66.
 // Method `async_wait()` on sockets and stream descriptors is preferred.
#undef ENABLE_NULL_BUFFERS
#endif

#ifdef ENABLE_NULL_BUFFERS
#define PHP_ASIO_BUFFER_LEN_VALIDATE() \
    if (UNEXPECTED(length < 0)) { \
        PHP_ASIO_ERROR(E_WARNING, "Non-negative integer expected."); \
        RETURN_NULL(); \
    }
#define PHP_ASIO_EMPTY_READ_BUFFER length == 0 ? ZSTR_EMPTY_ALLOC() :
#define PHP_ASIO_ON_READABLE(obj) \
    if (length == 0) \
        if (read_some) \
            (obj).async_read_some(boost::asio::null_buffers(), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
        else \
            async_read(obj, boost::asio::null_buffers(), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
    else
#define PHP_ASIO_EMPTY_WRITE_BUFFER ZSTR_LEN(data) == 0 ? nullptr :
#define PHP_ASIO_ON_WRITABLE(obj) \
    if (ZSTR_LEN(data) == 0) \
        if (write_some) \
            (obj).async_write_some(boost::asio::null_buffers(), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
        else \
            async_write(obj, boost::asio::null_buffers(), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
    else
#else
#define PHP_ASIO_BUFFER_LEN_VALIDATE() \
    if (UNEXPECTED(length <= 0)) { \
        PHP_ASIO_ERROR(E_WARNING, "Positive integer expected."); \
        RETURN_NULL(); \
    }
#define PHP_ASIO_EMPTY_READ_BUFFER
#define PHP_ASIO_ON_READABLE(obj)
#define PHP_ASIO_EMPTY_WRITE_BUFFER
#define PHP_ASIO_ON_WRITABLE(obj)
#endif // ENABLE_NULL_BUFFERS

#define PHP_ASIO_READ(type, obj) \
    zend_long length; \
    zend_bool read_some = 1; \
    zval* callback = nullptr; \
    zval* argument = nullptr; \
    ZEND_PARSE_PARAMETERS_START(1, 4) \
        Z_PARAM_LONG(length) \
        Z_PARAM_OPTIONAL \
        Z_PARAM_BOOL(read_some) \
        Z_PARAM_ZVAL(callback) \
        Z_PARAM_ZVAL(argument) \
        ZEND_PARSE_PARAMETERS_END(); \
    PHP_ASIO_BUFFER_LEN_VALIDATE(); \
    auto buffer_container = PHP_ASIO_EMPTY_READ_BUFFER \
        zend_string_alloc(static_cast<size_t>(length), 0); \
    PHP_ASIO_FUTURE_INIT(); \
    future->on_resolve<size_t>(boost::bind(&type::read_handler, \
        this, _1, _2, buffer_container, STRAND_UNWRAP(), args)); \
    PHP_ASIO_ON_READABLE(obj) \
        if (read_some) \
            (obj).async_read_some(mutable_buffer(buffer_container), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
        else \
            async_read(obj, mutable_buffer(buffer_container), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
    FUTURE_RETURN()

#define PHP_ASIO_WRITE(type, obj) \
    zend_string* data; \
    zend_bool write_some = 0; \
    zval* callback = nullptr; \
    zval* argument = nullptr; \
    ZEND_PARSE_PARAMETERS_START(1, 4) \
        Z_PARAM_STR(data) \
        Z_PARAM_OPTIONAL \
        Z_PARAM_BOOL(write_some) \
        Z_PARAM_ZVAL(callback) \
        Z_PARAM_ZVAL(argument) \
    ZEND_PARSE_PARAMETERS_END(); \
    auto buffer_container = PHP_ASIO_EMPTY_WRITE_BUFFER \
        zend_string_copy(data); \
    PHP_ASIO_FUTURE_INIT(); \
    future->on_resolve<size_t>(boost::bind(&type::write_handler, \
        this, _1, _2, buffer_container, STRAND_UNWRAP(), args)); \
    PHP_ASIO_ON_WRITABLE(obj) \
        if (write_some) \
            (obj).async_write_some(mutable_buffer(buffer_container), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
        else \
            async_write(obj, mutable_buffer(buffer_container), \
                STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(size_t))); \
    FUTURE_RETURN()

namespace asio
{
    /// Wrap a zend string into const buffer.
    inline auto const_buffer(const zend_string* str)
    {
        return boost::asio::const_buffers_1(boost::asio::const_buffer(
            ZSTR_LEN(str) ? ZSTR_VAL(str) : nullptr, ZSTR_LEN(str)
        ));
    }

    /// Wrap a zend string into mutable buffer.
    inline auto mutable_buffer(zend_string* str)
    {
        return boost::asio::mutable_buffers_1(boost::asio::mutable_buffer(
            ZSTR_LEN(str) ? ZSTR_VAL(str) : nullptr, ZSTR_LEN(str)
        ));
    }

    /// Extract a valid poll fd from a PHP resource.
    inline int resource_to_fd(zval* resource)
    {
        // The following code is copied from php-uv.
        // See https://github.com/bwoebi/php-uv/blob/master/php_uv.c#L401
        if (UNEXPECTED(Z_TYPE_P(resource) != IS_RESOURCE))
            return -1;
        const auto stream = static_cast<php_stream*>(
            zend_fetch_resource_ex(resource, nullptr, php_file_le_stream()));
        if (stream == nullptr)
            return -1;
        if (stream->wrapper && !strcmp(stream->wrapper->wops->label, "PHP") &&
            (!stream->orig_path ||
                strncmp(stream->orig_path, "php://std", sizeof "php://std" - 1) &&
                strncmp(stream->orig_path, "php://fd", sizeof "php://fd" - 1)))
            return -1;
        auto fd = -1;
        if (php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL,
            reinterpret_cast<void**>(&fd), 1) == SUCCESS && fd >= 0) {
            if (stream->wrapper && !strcmp(stream->wrapper->wops->label, "plainfile"))
                return -1;
            return fd;
        }
        return -1;
    }
}
