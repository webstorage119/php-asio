/**
 * php-asio/generator.hpp
 *
 * @author CismonX<admin@cismon.net>
 */

#pragma once

#include "common.hpp"

#include <zend_generators.h>

namespace Asio
{
    /* {{{ Functions copied from zend_generators.c */

    inline void zend_generator_ensure_initialized(zend_generator *generator)
    {
        if (UNEXPECTED(Z_TYPE(generator->value) == IS_UNDEF) &&
            EXPECTED(generator->execute_data) &&
            EXPECTED(generator->node.parent == nullptr)) {
            generator->flags |= ZEND_GENERATOR_DO_INIT;
            zend_generator_resume(generator);
            generator->flags &= ~ZEND_GENERATOR_DO_INIT;
            generator->flags |= ZEND_GENERATOR_AT_FIRST_YIELD;
        }
    }

    inline bool generator_valid(zend_generator *generator)
    {
        zend_generator_ensure_initialized(generator);
        zend_generator_get_current(generator);
        return EXPECTED(generator->execute_data != nullptr);
    }

    inline zval* generator_current(zend_generator* generator)
    {
        zend_generator_ensure_initialized(generator);
        const auto root = zend_generator_get_current(generator);
        if (EXPECTED(generator->execute_data != NULL && Z_TYPE(root->value) != IS_UNDEF))
            return &root->value;
        return nullptr;
    }

    static void generator_send(zend_generator *generator, zval* value)
    {
        zend_generator_ensure_initialized(generator);
        if (UNEXPECTED(!generator->execute_data))
            return;
        const auto root = zend_generator_get_current(generator);
        if (root->send_target)
            ZVAL_COPY(root->send_target, value);
        zend_generator_resume(generator);
    }

    /* }}} */
}
