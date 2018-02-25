/**
 * php-asio/resolver.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "resolver.hpp"
#include "future.hpp"

namespace Asio
{
    template <typename Protocol>
    zval* Resolver<Protocol>::handler(const boost::system::error_code& error,
        iterator iter, zval* callback, zval* argument)
    {
        iterator end;
        ZVAL_INIT(addr_list);
        ZVAL_NEW_ARR(&addr_list);
        zend_hash_init(Z_ARR(addr_list), 0, nullptr, ZVAL_PTR_DTOR, 0);
        ZVAL_INIT(addr_val);
        while (iter != end) {
            auto addr = (*iter++).endpoint().address().to_string();
            ZVAL_STR(&addr_val, zend_string_init(addr.c_str(), addr.length(), 0));
            zend_hash_next_index_insert(Z_ARR(addr_list), &addr_val);
        }
        PHP_ASIO_INVOKE_CALLBACK_START(4)
            ZVAL_ARR(&arguments[1], Z_ARR(addr_list));
            PHP_ASIO_INVOKE_CALLBACK();
        PHP_ASIO_INVOKE_CALLBACK_END();
        CORO_RETURN(ZVAL_ARR, Z_ARR(addr_list));
    }

    template <typename Protocol>
    P3_METHOD(Resolver<Protocol>, resolve)
    {
        zend_string* host;
        zend_string* service = nullptr;
        zval* callback = nullptr;
        zval* argument = nullptr;
        ZEND_PARSE_PARAMETERS_START(1, 4)
            Z_PARAM_STR(host)
            Z_PARAM_OPTIONAL
            Z_PARAM_STR(service)
            Z_PARAM_ZVAL(callback)
            Z_PARAM_ZVAL(argument)
            ZEND_PARSE_PARAMETERS_END();
        PHP_ASIO_FUTURE_INIT();
        future->on_resolve<iterator>(boost::bind(&Resolver::handler, this, _1, _2, STRAND_UNWRAP(), args));
        resolver_.async_resolve({ ZSTR_VAL(host), service ? ZSTR_VAL(service) : "" },
            STRAND_RESOLVE(ASYNC_HANDLER_DOUBLE_ARG(iterator)));
        FUTURE_RETURN();
    }

    template <typename Protocol>
    P3_METHOD(Resolver<Protocol>, cancel)
    {
        resolver_.cancel();
        PHP_ASIO_OBJ_DTOR();
    }

    template <typename Protocol>
    zend_class_entry* Resolver<Protocol>::class_entry;

    template <typename Protocol>
    zend_object_handlers Resolver<Protocol>::handlers;

    template class Resolver<tcp>;
    template class Resolver<udp>;
}
