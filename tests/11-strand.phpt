--TEST--
Test for `Strand`.
--SKIPIF--
<?php
if (!class_exists(Asio\Strand::class))
    echo 'skip Strand is not enabled';
?>
--FILE--
<?php
// Just test whether Strand works.
// We do not test multi-threading because is not yet supported.
$service = new Asio\Service;
$strand = $service->addStrand();
$strand->post(function () use ($service, $strand) {
    $timer = $service->addTimer();
    $timer->expiresFromNow(100);
    $timer->wait($strand->wrap(function () {
        echo 'ok';
    }));
});
$service->run();
?>
--EXPECT--
ok
