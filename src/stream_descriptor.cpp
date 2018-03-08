/**
 * php-asio/stream_descriptor.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "stream_descriptor.hpp"
#include "future.hpp"
#include "io.hpp"

namespace asio
{
    zval* stream_descriptor::read_handler(const boost::system::error_code& error,
        size_t length, zend_string* buffer, zval* callback, zval* argument)
    {
        ZSTR_VAL(buffer)[length] = '\0';
        ZSTR_LEN(buffer) = length;
        PHP_ASIO_INVOKE_CALLBACK_START(4)
            ZVAL_STR(&arguments[1], buffer);
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_STR, buffer);
    }

    zval* stream_descriptor::write_handler(const boost::system::error_code& error,
        size_t length, zend_string* buffer, zval* callback, zval* argument)
    {
#ifdef ENABLE_NULL_BUFFERS
        if (buffer)
#endif //ENABLE_NULL_BUFFERS
            zend_string_release(buffer);
        PHP_ASIO_INVOKE_CALLBACK_START(4)
            ZVAL_LONG(&arguments[1], static_cast<zend_long>(length));
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_LONG, static_cast<zend_long>(length));
    }

    P3_METHOD(stream_descriptor, assign)
    {
        zval* fd;
        ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ZVAL(fd)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        if (UNEXPECTED(Z_TYPE_P(fd) == IS_LONG))
            stream_descriptor_.assign(Z_LVAL_P(fd), ec);
        else
            stream_descriptor_.assign(resource_to_fd(fd), ec);
        RETVAL_EC(ec);
    }

    P3_METHOD(stream_descriptor, isOpen)
    {
        RETVAL_BOOL(stream_descriptor_.is_open());
    }

    P3_METHOD(stream_descriptor, read)
    {
        PHP_ASIO_READ(stream_descriptor, stream_descriptor_);
    }

    P3_METHOD(stream_descriptor, write)
    {
        PHP_ASIO_WRITE(stream_descriptor, stream_descriptor_);
    }

    P3_METHOD(stream_descriptor, release)
    {
        stream_descriptor_.release();
    }

    P3_METHOD(stream_descriptor, cancel)
    {
        boost::system::error_code ec;
        RETVAL_EC(stream_descriptor_.cancel(ec));
    }

    P3_METHOD(stream_descriptor, close)
    {
        boost::system::error_code ec;
        RETVAL_EC(stream_descriptor_.close(ec));
    }

    PHP_ASIO_CE_DEFINE(stream_descriptor);
}
