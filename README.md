# php-asio

[![Travis-CI](https://travis-ci.org/CismonX/php-asio.svg?branch=master)](https://travis-ci.org/CismonX/php-asio)
[![MIT license](https://img.shields.io/badge/licence-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## 1. About

This extension is, to some extent, a PHP wrapper for the Boost.Asio library, hence the name php-asio.

It provides a high-performance event-driven model for networking, which makes asynchronous and concurrency simple.

Now php-asio is **in development**, do not use it in production.

Bug reports and pull requests are welcome.

## 2. Documentation

See the [Wiki](https://github.com/CismonX/php-asio/wiki) page for documentation.

Signatures of classes and functions can be found within [stubs/](stubs/) directory, with PHPDoc.

There are also some test cases in the [tests/](tests/) directory.

## 3. TODO list

* Multi-threading support. (Priority: **high**)

* Fix memory leak (once in every 9,998 Zend objects). (Priority: **medium**)

* Add support for serial ports. (Priority: **low**)

* Add socket `onReadable()` `onWritable()` `onError()` methods (with Boost version 1.66 and above). (Priority: **low**)

* Add support for Windows. (Priority: **low**)