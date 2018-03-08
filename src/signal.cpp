/**
 * php-asio/signal.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "signal.hpp"
#include "future.hpp"

namespace asio
{
    zval* signal::handler(const boost::system::error_code& error,
        int signal, zval* callback, zval* argument)
    {
        PHP_ASIO_INVOKE_CALLBACK_START(4);
            ZVAL_LONG(&arguments[1], static_cast<zend_long>(signal));
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_LONG, static_cast<zend_long>(signal));
    }

    P3_METHOD(signal, add)
    {
        size_t sig_len = 0;
        zval* sig_num = nullptr;
        ZEND_PARSE_PARAMETERS_START(1, -1)
            Z_PARAM_VARIADIC('+', sig_num, sig_len)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        for (size_t i = 0; i < sig_len; i++) {
            if (Z_TYPE(sig_num[i]) != IS_LONG) {
                PHP_ASIO_ERROR(E_NOTICE, "Integer value expected.");
                continue;
            }
            signal_.add(static_cast<int>(Z_LVAL(sig_num[i])), ec);
            if (ec)
                break;
        }
        RETVAL_EC(ec);
    }

    P3_METHOD(signal, remove)
    {
        size_t sig_len = 0;
        zval* sig_num = nullptr;
        ZEND_PARSE_PARAMETERS_START(1, -1)
            Z_PARAM_VARIADIC('+', sig_num, sig_len)
        ZEND_PARSE_PARAMETERS_END();
        boost::system::error_code ec;
        for (size_t i = 0; i < sig_len; i++) {
            if (Z_TYPE(sig_num[i]) != IS_LONG) {
                PHP_ASIO_ERROR(E_NOTICE, "Integer value expected.");
                continue;
            }
            signal_.remove(static_cast<int>(Z_LVAL(sig_num[i])), ec);
            if (ec)
                break;
        }
        RETVAL_EC(ec);
    }

    P3_METHOD(signal, wait)
    {
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(0, 2)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
        ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_FUTURE_INIT();
        future->on_resolve<int>(boost::bind(
            &signal::handler, this, _1, _2, STRAND_UNWRAP(), args));
        signal_.async_wait(STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(int)));
        FUTURE_RETURN();
    }

    P3_METHOD(signal, clear)
    {
        boost::system::error_code ec;
        RETVAL_EC(signal_.clear(ec));
    }

    P3_METHOD(signal, cancel)
    {
        boost::system::error_code ec;
        RETVAL_EC(signal_.cancel(ec));
    }

    PHP_ASIO_CE_DEFINE(signal);
}
