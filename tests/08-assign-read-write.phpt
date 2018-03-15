--TEST--
Test for `assign()` on `Socket`, `Acceptor` and `StreamDescriptor`.
Test for `StreamSocket::read()` and `StreamSocket::write()`.
--SKIPIF--
<?php
if (!stream_socket_server('tcp://127.0.0.1:21348', $ec))
    echo 'skip ', posix_strerror($ec);
?>
--FILE--
<?php
$handle = stream_socket_server('tcp://127.0.0.1:21348');
$service = new Asio\Service;
$acceptor = $service->addTcpAcceptor();
if ($ec = $acceptor->assign(false, $handle))
    die('Acceptor::assign() failed. '.posix_strerror($ec));
$acceptor->accept(function ($acceptor, $socket, $ec) use ($service) {
    if ($ec) {
        echo 'Error on TcpAcceptor::accept(). ', posix_strerror($ec);
        return;
    }
    $socket->read(5, false, function ($socket, $data, $ec) use ($service) {
        if ($ec) {
            echo 'TcpSocket::read() failed.', posix_strerror($ec);
            return;
        }
        echo $data;
        $stream_descriptor = $service->addStreamDescriptor();
        $stream_descriptor->assign(STDOUT);
        $stream_descriptor->write('world');
    });
});
$service->post(function () use ($service) {
    $handle = stream_socket_client('tcp://127.0.0.1:21348');
    $socket = $service->addTcpSocket();
    $socket->assign(false, $handle);
    $socket->write('hello');
});
$service->run();
?>
--EXPECT--
helloworld
