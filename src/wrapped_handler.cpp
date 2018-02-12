/**
 * php-asio/wrapped_handler.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "wrapped_handler.hpp"

#ifdef ENABLE_STRAND
namespace Asio
{
    PHP_ASIO_CE_DEFINE(WrappedHandler);
}
#endif // ENABLE_STRAND
