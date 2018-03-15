--TEST--
Test for `DatagramSocket::sendTo()` and `DatagramSocket::recvFrom()`.
--FILE--
<?php
$service = new Asio\Service;
$service->post(function () use ($service) {
    $socket = $service->addUdgSocket();
    $socket->open();
    $socket->bind('/tmp/test.sock');
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
                    echo 'UdgSocket::sendTo failed. ', posix_strerror($ec);
                    $socket->close();
                    return;
                }
                $socket->close();
            });
        });
    $socket = $service->addUdgSocket();
    $socket->open();
    $socket->bind('/tmp/test2.sock');
    $socket->sendTo('hello', '/tmp/test.sock', function ($socket) {
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
--EXPECT--
helloworld
