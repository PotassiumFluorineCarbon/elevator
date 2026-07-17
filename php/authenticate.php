<?php 
session_start();

require 'database.php';

$inputUser = $_POST['username'] ?? '';  
$inputPass = $_POST['password'] ?? '';  

$authenticated = false;

if (!empty($inputUser) && !empty($inputPass)) {
    // Prepare the SQL statement to find the user
    $stmt = $conn->prepare("SELECT Password FROM Users WHERE Username = ?");
    $stmt->bind_param("s", $inputUser);
    $stmt->execute();
    
    $result = $stmt->get_result();
    
    if ($row = $result->fetch_assoc()) {
        // Verify the password. 
        // This checks both hashed passwords (new accounts) AND plain text passwords 
        // just in case your SQL backup currently contains plain-text passwords.
        if (password_verify($inputPass, $row['Password']) || $inputPass === $row['Password']) {
            $authenticated = true;
        }
    }
    $stmt->close();
}

if ($authenticated) {
    // Set session variable
    $_SESSION['username'] = $inputUser;
    
    // Auto-redirect to members page
    header("Location: member.php");
    exit();
} else {
    // Invalid credentials flow
    echo "<p>Invalid credentials. Please click <a href='../html/login.html'>here</a> to try again.</p>";
}
?>
