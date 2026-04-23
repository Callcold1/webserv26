<?php
$method = $_SERVER['REQUEST_METHOD'];

if ($method === 'POST') {
    $name = isset($_POST['firstname']) ? $_POST['firstname'] : '';
} else {
    $name = isset($_GET['firstname']) ? $_GET['firstname'] : '';
}
echo "Status: 200 OK\r\n";
echo "Content-type: text/html\r\n\r\n";

echo "<!doctype html>";
echo "Hello, " . $name;
?>