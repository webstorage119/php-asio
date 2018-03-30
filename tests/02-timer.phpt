--TEST--
Test for `Timer`.
--FILE--
<?php
$service = new Asio\Service;
$timer = $service->addTimer();
if ($ec = $timer->expiresFromNow(200))
    die('Timer::expire() failed. '.posix_strerror($ec));
$start_time = microtime(true);
$timer->wait(function ($timer, $ec) use ($start_time) {
    if ($ec) {
        echo 'Error on Timer::wait(). ', posix_strerror($ec);
        return;
    }
    $end_time = microtime(true);
    $duration = intval(1000 * ($end_time - $start_time));
    if ($duration != 200)
        echo "Bad duration. Expected 200ms, got ${duration}ms";
});
$service->run($ec);
?>
--EXPECT--
