--TEST--
Test for `Resolver`.
--FILE--
<?php
$service = new Asio\Service;
$service->post(function () use ($service) {
    $resolver = $service->addTcpResolver();
    $resolver->resolve('github.com', 'http', function ($resolver, $arr, $ec) {
        if ($ec = Asio\Service::lastError()) {
            echo 'Resolver::resolve() failed. ', posix_strerror($ec);
            return;
        }
        echo boolval(count($arr));
    });

});
$service->run();
?>
--EXPECTF--
1
