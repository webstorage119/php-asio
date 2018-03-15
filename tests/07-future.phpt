--TEST--
Test for `Future` and `Service::lastError()`.
--SKIPIF--
<?php
if (!class_exists('Asio\Future'))
    echo 'skip coroutine is disabled';
?>
--FILE--
<?php
$service = new Asio\Service;
$service->post(function () use ($service) {
    $signal = $service->addSignal();
    $signal->add(SIGINT);
    $service->post(function () {
        posix_kill(posix_getpid(), SIGINT);
    });
    echo yield $signal->wait();
});
$service->run();
?>
--EXPECT--
2
