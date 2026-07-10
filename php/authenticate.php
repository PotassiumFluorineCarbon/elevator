<?php 
session_start();

$username = $_POST['username'] ?? '';  
$password = $_POST['password'] ?? '';  

// Initialize variable to prevent undefined warnings
$authenticated = false;

// Load users from JSON
$jsonData = @file_get_contents('../json/users.json');
$users = json_decode($jsonData, true);

if ($users === null) {
    die("<p>Error: Could not load user data.</p>");
}

// Check each user in the array
foreach ($users as $user) {
    if ($user['username'] === $username && $user['password'] === $password) {
        $authenticated = true;
        break;
    }
}

if ($authenticated) {
    // Set session variable
    $_SESSION['username'] = $username;
    
    // Auto-redirect to members page (from your working project logic)
    header("Location: member.php");
    exit();
} else {
    // Invalid credentials flow
    echo "<p>Invalid credentials. Please click <a href='../html/login.html'>here</a> to try again.</p>";
}
?>