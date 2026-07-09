<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);

// 1. Point to your new project's JSON file
$usersFile = '../json/users.json';

// 2. Read existing users (or create an empty array if the file is missing/empty)
$usersData = file_exists($usersFile) ? file_get_contents($usersFile) : '[]';
$usersArray = json_decode($usersData, true);

if (!is_array($usersArray)) {
    $usersArray = [];
}

// 3. Grab the submitted credentials
$newUser = array(
    "username" => $_POST['username'] ?? '',
    "password" => $_POST['password'] ?? '',
);

// 4. (New Step!) Check if the username already exists to prevent duplicates
$userExists = false;
foreach ($usersArray as $user) {
    if (strtolower($user['username']) === strtolower($newUser['username'])) {
        $userExists = true;
        break;
    }
}

// 5. Append and save ONLY if the user doesn't exist and the fields aren't empty
if (!$userExists && !empty($newUser['username']) && !empty($newUser['password'])) {
    $usersArray[] = $newUser;
    file_put_contents($usersFile, json_encode($usersArray, JSON_PRETTY_PRINT));
}

header("Location: ../html/login.html");
exit();
?>