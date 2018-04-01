/**
 * php-asio/php_asio.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

extern zend_module_entry asio_module_entry;
#define phpext_asio_ptr &asio_module_entry

#define PHP_ASIO_VERSION "0.1.5"

#if defined(__GNUC__) && __GNUC__ >= 4
#define PHP_ASIO_API __attribute__ ((visibility("default")))
#else
#define PHP_ASIO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_ASIO)
ZEND_TSRMLS_CACHE_EXTERN()
#endif
