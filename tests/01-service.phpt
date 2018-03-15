--TEST--
Test for `Service`.
--FILE--
<?php
$service = new Asio\Service();
$service->dispatch(function () use ($service) {
    echo 'Service::dispatch()', PHP_EOL;
    $service->post(function ($arg) use ($service) {
        echo "Service::post($arg)";
    }, 'foo');
});
$service->run($ec);
if ($ec)
    echo 'Error on Service::run(). ', posix_strerror($ec);
?>
--EXPECT--
Service::dispatch()
Service::post(foo)
