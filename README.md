# php-asio

[![Travis-CI](https://travis-ci.org/CismonX/php-asio.svg?branch=master)](https://travis-ci.org/CismonX/php-asio)
[![MIT license](https://img.shields.io/badge/licence-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## 1. About

This extension is, to some extent, a PHP wrapper for the Boost.Asio library, hence the name php-asio.

It provides a high-performance event-driven model for networking, which makes asynchronous and concurrency simple.

Now php-asio is **in development**, do not use it in production.

Bug reports and pull requests are welcome.

## 2. Build

### 2.1 Requirements

* PHP Version >= 7.0
* The [Boost](http://www.boost.org/) C++ libraries (Version >= 1.55.0)
* C++ compiler which supports C++14

### 2.2 Notes

* If you don't need coroutines, you can `--disable-asio-coroutine` for better performance.
* If you are writing multi-threaded programs, you can `--enable-asio-strand` for `Strand` support.
* Windows is not yet supported.

## 3. Documentation

Signatures of classes and functions can be found within [stubs/](stubs/) directory, with PHPDoc.

There are also some examples in the [examples/](examples/) directory.

A detailed documentation will be coming soon once the first stable version is released.

## 4. TODO list

* Fix memory leak (once in every 9,998 Zend objects). (Priority: **high**)

* Finish `StreamDescriptor`. (Priority: **medium**)

* Add socket `onReadable()` `onWritable()` `onError()` methods (with Boost version 1.66 and above). (Priority: **low**)

* Add support for Windows. (Priority: **low**)