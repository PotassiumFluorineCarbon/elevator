<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

$usersFile = '../json/users.json';

$usersData = file_exists($usersFile) ? file_get_contents($usersFile) : '[]';
$usersArray = json_decode($usersData, true);

if (!is_array($usersArray)) {
    $usersArray = [];
}

$newUser = array(
    "username" => $_POST['username'] ?? '',
    "password" => $_POST['password'] ?? '',
);

$userExists = false;
foreach ($usersArray as $user) {
    if (strtolower($user['username']) === strtolower($newUser['username'])) {
        $userExists = true;
        break;
    }
}

if (!$userExists && !empty($newUser['username']) && !empty($newUser['password'])) {
    $usersArray[] = $newUser;
    file_put_contents($usersFile, json_encode($usersArray, JSON_PRETTY_PRINT));
}

header("Location: ../html/login.html");
exit();
?>