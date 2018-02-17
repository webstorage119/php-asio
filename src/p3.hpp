/**
 * p3.hpp
 *
 * This header is a simple helper for wrapping C++ classes,
 * which is borrowed from https://github.com/phplang/p3.
 * The casting/cloning/comparing functionalities are removed,
 * because they are not needed by php-asio.
 */

#pragma once

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <php.h>
#include <zend_exceptions.h>

#include <new>
#include <type_traits>

#define P3_METHOD_DECLARE(name) \
  void zim_##name(INTERNAL_FUNCTION_PARAMETERS)

#define P3_METHOD(cls, name) \
  void cls::zim_##name(INTERNAL_FUNCTION_PARAMETERS)

#define P3_ME(cls, name, meth, arginfo, flags) \
  ZEND_FENTRY(name, [](INTERNAL_FUNCTION_PARAMETERS) { \
      ::p3::toObject<cls>(getThis())->zim_##meth(INTERNAL_FUNCTION_PARAM_PASSTHRU); \
  }, arginfo, flags)

#define P3_ME_D(cls, meth, arginfo, flags) \
  P3_ME(cls, meth, meth, arginfo, flags)

#define P3_STATIC_ME(cls, meth, arginfo, flags) \
  ZEND_FENTRY(meth, &cls::zim_##meth, arginfo, flags | ZEND_ACC_STATIC)

#define P3_ABSTRACT_ME(name, arginfo) \
    PHP_ABSTRACT_ME("", name, arginfo)

namespace p3 {

    template <class T>
    zend_object* toZendObject(T* obj)
    {
        return reinterpret_cast<zend_object*>(obj + 1);
    }

    template <class T>
    T* toObject(zend_object* obj)
    {
        return reinterpret_cast<T*>(obj) - 1;
    }

    template <class T>
    T* toObject(zval* obj)
    {
        return reinterpret_cast<T*>(Z_OBJ_P(obj)) - 1;
    }

    template <class T, typename InitFunc>
    zend_object* allocObject(zend_class_entry* ce, InitFunc init)
    {
        auto ptr = reinterpret_cast<T*>(ecalloc(1, sizeof(T) +
            sizeof(zend_object) + zend_object_properties_size(ce)));
        init(ptr);
        auto zobj = toZendObject(ptr);
        zend_object_std_init(zobj, ce);
        zobj->handlers = &T::handlers;
        return zobj;
    }

    template <class T>
    typename std::enable_if<std::is_constructible<T>::value, zend_object*>::type
        createObject(zend_class_entry* ce)
    {
        return allocObject<T>(ce, [](T* ptr) {
            new(ptr) T();
        });
    }

    template <class T>
    typename std::enable_if<!std::is_constructible<T>::value, zend_object*>::type
        createObject(zend_class_entry* ce)
    {
        assert(false);
        return nullptr;
    }

    template <class T>
    void dtorObject(zend_object *obj)
    {
        zend_object_std_dtor(obj);
        auto cobj = toObject<T>(obj);
        cobj->~T();
        efree(cobj);
    }

    template <class T>
    zend_object* createThrownObject(zend_class_entry* ce) {
        zend_throw_exception_ex(zend_ce_error, 0,
            "%s may not be directly instantiated", ZSTR_VAL(ce->name));
        return zend_objects_new(ce);
    }

    template <class T>
    zend_class_entry* initClassEntry(const char* name, const zend_function_entry* methods)
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY_EX(ce, name, strlen(name), methods);
        T::class_entry = zend_register_internal_class(&ce);
        T::class_entry->create_object =
            std::is_constructible<T>::value ? createObject<T> : createThrownObject<T>;
        memcpy(&T::handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        T::handlers.offset = sizeof(T);
        T::handlers.free_obj = dtorObject<T>;
        T::handlers.clone_obj = nullptr;
        return T::class_entry;
    }

    template <>
    inline zend_class_entry* initClassEntry<void>(const char* name, const zend_function_entry* methods)
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY_EX(ce, name, strlen(name), methods);
        return zend_register_internal_interface(&ce);
    }
}
