<?php 
session_start();
session_unset();
session_destroy();

// Redirect automatically to the login page
header("Location: ../html/login.html");
exit();
?>