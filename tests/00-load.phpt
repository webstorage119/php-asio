--TEST--
Check for whether php-asio is loaded.
--FILE--
<?php
if (!extension_loaded('asio'))
    echo 'php-asio not loaded.'
?>
--EXPECT--
