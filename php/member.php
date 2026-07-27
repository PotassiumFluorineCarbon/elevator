<?php

error_reporting(E_ALL);
ini_set('display_errors', 1);

session_start();


if (!isset($_SESSION['username'])) {

    header("Location: ../html/login.html");

    exit();

}

require_once __DIR__ . "/databaseFunctions.php";




$path = "mysql:host=localhost;dbname=elevator";
$user = "elevator_user";
$password = "password";




// INSERT

if(isset($_POST['insert']))
{

    insert(
        $path,
        $user,
        $password,
        $_POST['NodeName'],
        $_POST['nodeType'],
        $_POST['NetworkType'],
        $_POST['Status']
    );

}





// UPDATE

if(isset($_POST['update']))
{

    update(
        $path,
        $user,
        $password,
        "elevatorNetwork",
        $_POST['nodeID'],
        $_POST['NodeName'],
        $_POST['nodeType'],
        $_POST['NetworkType'],
        $_POST['Status']
    );

}





// DELETE

if(isset($_POST['delete']))
{

    delete(
        $path,
        $user,
        $password,
        "elevatorNetwork",
        $_POST['nodeID']
    );

}



?>


<!DOCTYPE html>

<html>

<head>
    <title>Members Only</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../CSS/style.css">
</head>

<header class="bg-dark text-white py-4 mb-3">
    <div class="container text-center">
        <h1 class="display-5 fw-bold">Elevator Control System</h1>
    </div>
</header>

<nav class="container mb-4">
    <ul class="nav nav-pills justify-content-center bg-white py-2 rounded shadow">

        <li class="nav-item">
            <a class="nav-link text-dark" href="controlPanel.php">
                Control Panel
            </a>
        </li>

        <li class="nav-item">
            <a class="nav-link text-dark" href="diagnostics.php">
                Diagnostics
            </a>
        </li>

        <li class="nav-item">
            <a class="nav-link text-dark" href="messages.php">
                Messages
            </a>
        </li>

        <li class="nav-item">
            <a class="nav-link active bg-dark text-white" href="member.php">
                Members
            </a>
        </li>

        <li class="nav-item">
            <a class="nav-link text-dark" href="logout.php">
                Logout
            </a>
        </li>

    </ul>
</nav>

<body class="control-panel">

<p>
Welcome,
<?php echo htmlspecialchars($_SESSION['username']); ?>
</p>

<h4>Elevator Network</h4>



<?php

showtable(
    $path,
    $user,
    $password,
    "elevatorNetwork"
);

?>



<h4>Add Node</h4>

<form method="post">

<fieldset>

<p>
<label for="NodeName">Node Name:</label>
<input name="NodeName" id="NodeName">
</p>

<p>
<label for="nodeType">Node Type:</label>
<input name="nodeType" id="nodeType">
</p>

<p>
<label for="NetworkType">Network Type:</label>
<input name="NetworkType" id="NetworkType">
</p>

<p>
<label for="Status">Status:</label>
<input name="Status" id="Status">
</p>

<p>
<button name="insert">INSERT</button>
</p>

</fieldset>

</form>


<h4>Update/Delete Node</h4>

<form method="post">

<fieldset>

<p>
<label for="nodeID">Node ID:</label>
<input name="nodeID" id="nodeID">
</p>

<p>
<label for="NodeName2">Node Name:</label>
<input name="NodeName" id="NodeName2">
</p>

<p>
<label for="nodeType2">Node Type:</label>
<input name="nodeType" id="nodeType2">
</p>

<p>
<label for="NetworkType2">Network Type:</label>
<input name="NetworkType" id="NetworkType2">
</p>

<p>
<label for="Status2">Status:</label>
<input name="Status" id="Status2">
</p>

<p>
<button name="update">UPDATE</button>

<button name="delete">DELETE</button>
</p>

</fieldset>

</form>
