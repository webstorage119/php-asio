--TEST--
Test for `Signal`.
--FILE--
<?php
$service = new Asio\Service;
$signal = $service->addSignal();
if ($ec = $signal->add(SIGINT))
    die('Signal::add() failed. '.posix_strerror($ec));
$signal->wait(function ($signal, $sig_num, $ec) use ($service) {
    if ($ec) {
        echo 'Error on Timer::wait(). ', posix_strerror($ec);
        return;
    }
    echo $sig_num;
});
$service->post(function () {
    if (!posix_kill(posix_getpid(), SIGINT))
        exit;
});
$service->run();
?>
--EXPECT--
2
