/**
 * php-asio/include/buffer.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

namespace Asio
{
    inline boost::asio::const_buffers_1 const_buffer(const zend_string* str)
    {
        return boost::asio::const_buffers_1(boost::asio::const_buffer(
            ZSTR_LEN(str) ? ZSTR_VAL(str) : nullptr, ZSTR_LEN(str)
        ));
    }

    inline boost::asio::mutable_buffers_1 mutable_buffer(zend_string* str)
    {
        return boost::asio::mutable_buffers_1(boost::asio::mutable_buffer(
            ZSTR_LEN(str) ? ZSTR_VAL(str) : nullptr, ZSTR_LEN(str)
        ));
    }
}