<?php
session_start();

// Kick user back to login if they bypassed the login screen
if (!isset($_SESSION['username'])) {
    header("Location: ../html/login.html");
    exit();
}
?>
<!DOCTYPE html>
<html>
<head>
    <title>Members Only</title>
</head>
<body>
    <p>Welcome, <?php echo htmlspecialchars($_SESSION['username']); ?>!</p>
    
    <p>Members only content - for your eyes only</p>

    <p>Click to <a href='logout.php'>Logout</a></p>
</body>
</html>