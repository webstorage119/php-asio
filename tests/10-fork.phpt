--TEST--
Test for forking.
--FILE--
<?php
$service = new Asio\Service;
function fork_worker($service, $signal)
{
    if ($ec = $service->forkPrepare())
        die('Service::forkParent() failed. '. posix_strerror($ec));
    if (pcntl_fork()) {
        if ($ec = $service->forkParent())
            die('Service::forkParent() failed. '. posix_strerror($ec));
        $signal->wait(function () {
            if (pcntl_wait($status, WUNTRACED) < 1)
                die('An error occurred during wait().');
            echo $status;
        });
        echo 'parent ';
    } else {
        if ($ec = $service->forkChild())
            die('Service::forkParent() failed. '. posix_strerror($ec));
        $signal->cancel();
        $timer = $service->addTimer();
        $timer->expiresFromNow(100);
        $timer->wait($timer_cb = function ($timer) use (&$timer_cb) {
            echo 'child ';
        });
    }
}
$service->post(function () use ($service) {
    $signal = $service->addSignal();
    $signal->add(SIGCHLD);
    fork_worker($service, $signal);
});
$service->run();
?>
--EXPECT--
parent child 0
