/**
 * php-asio/wrapped_handler.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "wrapped_handler.hpp"

#ifdef ENABLE_STRAND
namespace asio
{
    PHP_ASIO_CE_DEFINE(wrapped_handler);
}
#endif // ENABLE_STRAND
