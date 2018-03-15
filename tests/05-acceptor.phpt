--TEST--
Test for `Acceptor`.
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
$acceptor = $service->addTcpAcceptor();
if ($ec = $acceptor->open(false))
    die('TcpAcceptor::open() failed. '.posix_strerror($ec));
if ($ec = $acceptor->bind('127.0.0.1', 21348))
    die('TcpAcceptor::bind() failed. '.posix_strerror($ec));
if ($ec = $acceptor->listen())
    die('TcpAcceptor::listen() failed. '.posix_strerror($ec));
$acceptor->accept(function ($acceptor, $socket, $ec) use ($service) {
    if ($ec) {
        echo 'Error on TcpAcceptor::accept. ', posix_strerror($ec);
        return;
    }
    echo 'Accepted', PHP_EOL;
    $service->post(function () use ($service) {
        $acceptor = $service->addUnixAcceptor();
        if ($ec = $acceptor->open())
            die('UnixAcceptor::open() failed. '.posix_strerror($ec));
        if ($ec = $acceptor->bind(getenv('SOCK_FILE')))
            die('UnixAcceptor::bind() failed. '.posix_strerror($ec));
        if ($ec = $acceptor->listen())
            die('UnixAcceptor::listen() failed. '.posix_strerror($ec));
        $acceptor->accept(function ($acceptor, $socket, $ec) {
            if ($ec) {
                echo 'Error on UnixAcceptor::accept. ', posix_strerror($ec);
                return;
            }
            echo 'Accepted';
        });
        $service->post(function () use ($service) {
            if ($handle = stream_socket_client('unix://'.getenv('SOCK_FILE')))
                fclose($handle);
            else
                exit;
        });
    });
});
$service->post(function () use ($service) {
    if ($handle = stream_socket_client('tcp://127.0.0.1:21348'))
        fclose($handle);
    else
        exit;
});
$service->run();
?>
--CLEAN--
<?php
unlink(getenv('SOCK_FILE'));
?>
--EXPECT--
Accepted
Accepted
