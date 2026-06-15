<?php
    /*$submitted = !empty($_POST);
    if ($submitted == 1){
    $username = $_POST['username'];
    $password = $_POST['password'];
    setcookie('username', $username);
    setcookie('password', $password);
    } else{
        $username = $_COOKIE['username'];
		$password = $_COOKIE['password'];
    }

    echo "<p>Form submitted successfully (1 for true): $submitted</p>";
    echo "<p>Username received: $username</p>";
    echo "<p>Password received: $password</p>";
*/
?>

<?php 
	/*// authenticate.php (modified to use sessions)
	session_start(); // starts a session and creates a session variable
	$username = $_POST['username'];  
	$password = $_POST['password'];	 

	if($username&&$password){           // if($user === "mike" && $password === "mypassword") {     // Hard coded
		$_SESSION['username']=$username;
        $_SESSION['password']=$password;
		echo "<p>Congratulations, you are now logged into the site.</p>";
		echo "<p>Please click <a href=\"member.php\">here</a> to be taken to our members only page</p>";
	} else {
		echo "<p>Please enter a username and password</p>";
	}
	// Note: we have not checked to see if the username and password are valid
	// we will add this functionality when we study databases later*/
?>

<?php 
	// authenticate.php (modified to use sessions)
	session_start(); // starts a session and creates a session variable
	$username = $_POST['username'];  
	$password = $_POST['password'];	 

	// Load users from JSON
	$jsonData = file_get_contents('../json/users.json');
	$users = json_decode($jsonData, true);

	if ($users === null) {
    	echo "<p>Error: Could not load user data.</p>";
    	exit;
	}

	// Check each user in the array
	foreach ($users as $user) {
		if ($user['username'] === $username && $user['password'] === $password) {
			$authenticated = true;
			break;
		}
	}

	if($authenticated){           // if($user === "mike" && $password === "mypassword") {     // Hard coded
		$_SESSION['username']=$username;
        $_SESSION['password']=$password;
		echo "<p>Congratulations, you are now logged into the site.</p>";
		echo "<p>Please click <a href=\"member.php\">here</a> to be taken to our members only page</p>";
	} else {
		echo "<p>Please enter a username and password</p>";
	}
	// Note: we have not checked to see if the username and password are valid
	// we will add this functionality when we study databases later
?>