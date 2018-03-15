--TEST--
Test for binding socket to an endpoint and opening socket.
--ENV--
SOCK_FILE=/tmp/test-asio-socket-bind.sock
--SKIPIF--
<?php
if (!stream_socket_server('tcp://127.0.0.1:21348', $ec))
    echo 'skip ', posix_strerror($ec);
?>
--FILE--
<?php
$service = new Asio\Service;

$socket = $service->addTcpSocket();
if ($ec = $socket->open(false))
    die('TcpSocket::open() failed. '.posix_strerror($ec));
if ($ec = $socket->bind('127.0.0.1', 21348))
    die('TcpSocket::bind() failed. '.posix_strerror($ec));
$socket->close();

$socket = $service->addUdpSocket();
if ($ec = $socket->open(false))
    die('UdpSocket::open() failed. '.posix_strerror($ec));
if ($ec = $socket->bind('127.0.0.1', 21348))
    die('UdpSocket::bind() failed. '.posix_strerror($ec));
$socket->close();

$socket = $service->addUnixSocket();
if ($ec = $socket->open())
    die('UnixSocket::open() failed. '.posix_strerror($ec));
if ($ec = $socket->bind(getenv('SOCK_FILE')))
    die('UnixSocket::bind() failed. '.posix_strerror($ec));
$socket->close();

$socket = $service->addUdgSocket();
if ($ec = $socket->open())
    die('UdgSocket::open() failed. '.posix_strerror($ec));
if ($ec = $socket->bind(getenv('SOCK_FILE')))
    die('UdgSocket::bind() failed. '.posix_strerror($ec));
$socket->close();
?>
--CLEAN--
<?php
unlink(getenv('SOCK_FILE'));
?>
--EXPECT--
