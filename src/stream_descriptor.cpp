/**
 * php-asio/stream_descriptor.cpp
 *
 * @author CismonX<admin@cismon.net>
 */

#include "stream_descriptor.hpp"
#include "future.hpp"

namespace Asio
{
    int StreamDescriptor::resource_to_fd(zval* resource)
    {
        // The following code is copied from php-uv.
        // See https://github.com/bwoebi/php-uv/blob/master/php_uv.c#L401
        auto fd = -1;
        if (UNEXPECTED(Z_TYPE_P(resource) != IS_RESOURCE))
            return -1;
        auto stream = static_cast<php_stream*>(
            zend_fetch_resource_ex(resource, nullptr, php_file_le_stream()));
        if (stream == nullptr)
            return -1;
        if (stream->wrapper && !strcmp(stream->wrapper->wops->label, "PHP") && (!stream->orig_path ||
            (strncmp(stream->orig_path, "php://std", sizeof "php://std" - 1) &&
                strncmp(stream->orig_path, "php://fd", sizeof "php://fd" - 1)))) {
            return -1;
        }
        if (php_stream_cast(stream, PHP_STREAM_AS_FD_FOR_SELECT | PHP_STREAM_CAST_INTERNAL,
            reinterpret_cast<void**>(&fd), 1) == SUCCESS && fd >= 0) {
            if (stream->wrapper && !strcmp(stream->wrapper->wops->label, "plainfile"))
                return -1;
            return fd;
        }
        return -1;
    }

    PHP_ASIO_CE_DEFINE(StreamDescriptor);
}
