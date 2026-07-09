<?php

error_reporting(E_ALL);
ini_set('display_errors', 1);

include "database.php";


if(isset($_POST["floor"]))
{
    $floor = intval($_POST["floor"]);

    $sql = "
    INSERT INTO requests
    (
        floor,
        direction,
        processed
    )
    VALUES
    (
        ?,
        NULL,
        FALSE
    )
    ";

    $stmt = $conn->prepare($sql);
    $stmt->bind_param("i", $floor);

}


else if(isset($_POST["direction"]))
{
    $direction = $_POST["direction"];

    $sql = "
    INSERT INTO requests
    (
        floor,
        direction,
        processed
    )
    VALUES
    (
        NULL,
        ?,
        FALSE
    )
    ";

    $stmt = $conn->prepare($sql);
    $stmt->bind_param("s", $direction);

}


else
{
    die("No request received");
}


if($stmt->execute())
{
    echo "Request added";
}
else
{
    echo "Error: " . $stmt->error;
}


$stmt->close();
$conn->close();

?>