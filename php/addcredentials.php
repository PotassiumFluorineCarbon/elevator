<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

// database connection
require 'database.php';

$username = $_POST['username'] ?? '';
$password = $_POST['password'] ?? '';

if (!empty($username) && !empty($password)) {
    // Check if the username already exists in the database
    $checkStmt = $conn->prepare("SELECT UserID FROM Users WHERE Username = ?");
    $checkStmt->bind_param("s", $username);
    $checkStmt->execute();
    $result = $checkStmt->get_result();
    
    // If num_rows is 0, the username is available
    if ($result->num_rows === 0) {
        // Hash the password for security before storing it
        $hashedPassword = password_hash($password, PASSWORD_DEFAULT);
        
        // Insert the new user into the Users table
        $insertStmt = $conn->prepare("INSERT INTO Users (Username, Password) VALUES (?, ?)");
        $insertStmt->bind_param("ss", $username, $hashedPassword);
        $insertStmt->execute();
        $insertStmt->close();
    }
    $checkStmt->close();
}

$conn->close();

header("Location: ../html/login.html");
exit();
?>
