<?php
/**
 * Example for php-asio timer.
 *
 * @author CismonX<admin@cismon.net>
 */

$service = new Asio\Service;
$timer = $service->addTimer();
$timer->expire(1000);
$timer->wait(
    $timer_cb = function (Asio\Timer $timer, $ec, $arg) use ($service, &$timer_cb) {
        if ($ec)
            return;
        static $counter = 0;
        echo $arg, PHP_EOL;
        if (++$counter >= 5) {
            $timer->cancel();
            return;
        }
        // Using timestamp.
        $timer->expire(round(1000 * (microtime(true) + 1)), true);
        $timer->wait($timer_cb, 'Tick.');
    }, 'Tick.');
$service->run();
