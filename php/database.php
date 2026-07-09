<?php

$servername = "localhost";
$username = "elevator_user";
$password = "password";
$dbname = "elevator";


$conn = new mysqli(
    $servername,
    $username,
    $password,
    $dbname
);


if ($conn->connect_error)
{
    die("Database connection failed: " . $conn->connect_error);
}

?>