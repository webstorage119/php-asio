<?php
/**
 * Example for php-asio fork support.
 *
 * @author CismonX<admin@cismon.net>
 */

global $service;
$service = new Asio\Service;

function fork_worker(Asio\Signal $signal)
{
    global $service;
    $service->forkPrepare();
    $pid = pcntl_fork();
    if ($pid) {
        // Parent process.
        $service->forkParent();
        cli_set_process_title('php-asio Master');
        $signal->wait('handle_sigchld');
        echo 'New worker forked. Process ID: ', $pid, PHP_EOL;
    } else {
        // Child process.
        $service->forkChild();
        cli_set_process_title('php-asio Worker');
        // The child process is not interested in processing the SIGCHLD signal.
        $signal->cancel();
        $timer = $service->addTimer();
        $timer->expire(1000);
        $timer->wait($timer_cb = function (Asio\Timer $timer) use (&$timer_cb) {
            echo 'Tick.', PHP_EOL;
            $timer->expire(1000);
            $timer->wait($timer_cb);
        });
    }
}

function handle_sigchld(Asio\Signal $signal)
{
    $pid = pcntl_wait($status, WUNTRACED);
    if ($pid < 1)
        die('An error occurred during wait().');
    echo 'Worker ', $pid, ' exited with status ', $status, PHP_EOL;
    // Fork a new worker if previous one exited.
    fork_worker($signal);
}

$service->post(function () use ($service) {
    $signal = $service->addSignal();
    $signal->add(SIGCHLD);
    fork_worker($signal);
});
$service->run();