--TEST--
Test for `DatagramSocket::sendTo()` and `DatagramSocket::recvFrom()`.
--ENV--
SOCK_FILE_1=/tmp/test-asio-socket-bind-1.sock
SOCK_FILE_2=/tmp/test-asio-socket-bind-2.sock
--FILE--
<?php
$service = new Asio\Service;
$service->post(function () use ($service) {
    $socket = $service->addUdgSocket();
    $socket->open();
    $socket->bind(getenv('SOCK_FILE_1'));
    $socket->recvFrom(100,
        function ($socket, $data, $remote_path, $ec) {
            if ($ec) {
                echo 'UdgSocket::recvFrom() failed. ', posix_strerror($ec);
                $socket->close();
                return;
            }
            echo $data;
            $socket->sendTo('world', $remote_path, function ($socket, $length, $ec) {
                if ($ec) {
                    echo 'UdgSocket::sendTo() failed. ', posix_strerror($ec);
                    $socket->close();
                    return;
                }
                $socket->close();
            });
        });
    $socket = $service->addUdgSocket();
    $socket->open();
    $socket->bind(getenv('SOCK_FILE_2'));
    $socket->sendTo('hello', getenv('SOCK_FILE_1'), function ($socket) {
        $socket->recvFrom(100, function ($socket, $data, $remote_path, $ec) {
            if ($ec) {
                echo 'UdgSocket::recvFrom() failed. ', posix_strerror($ec);
                $socket->close();
                return;
            }
            echo $data;
            $socket->close();
        });
    });
});
$service->run();
?>
--CLEAN--
<?php
unlink(getenv('SOCK_FILE_1'));
unlink(getenv('SOCK_FILE_2'));
?>
--EXPECT--
helloworld
