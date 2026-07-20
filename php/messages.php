<?php
error_reporting(E_ALL);
ini_set('display_errors',1);
session_start();

if(!isset($_SESSION['username'])){
    header("Location: ../html/login.html");
    exit();
}

require_once "databaseFunctions.php";

$path="mysql:host=localhost;dbname=elevator";
$user="elevator_user";
$password="password";


if(isset($_POST["deleteMessage"]))
{
    clearMessages(
        $path,
        $user,
        $password,
        $_POST["messageID"]
    );

    header("Location: messages.php");
    exit();
}


$db = connect($path,$user,$password);


$messages = $db->query(
    "SELECT * FROM CAN_messages ORDER BY Timestamp DESC"
);

?>


<!DOCTYPE html>
<html>

<head>

<meta charset="utf-8">

<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">

<title>Messages</title>


<style>

body{
    font-family:Arial;
    background:#eee;
    margin:30px;
}


table{
    border-collapse:collapse;
    background:white;
    width:100%;
}


td,th{
    border:1px solid #555;
    padding:8px;
}


button{
    width:100px;
}


</style>


</head>


<body>


<header class="bg-dark text-white py-4 mb-3">

    <div class="container text-center">

        <h1 class="display-5 fw-bold">
            Elevator Control System
        </h1>

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
<a class="nav-link active bg-dark text-white" href="messages.php">
Messages
</a>
</li>


<li class="nav-item">
<a class="nav-link text-dark" href="member.php">
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



<h2>
Welcome
<?= htmlspecialchars($_SESSION["username"]) ?>
</h2>



<h3>CAN Messages</h3>



<table>


<tr>

<th>Message ID</th>

<th>CAN ID</th>

<th>Message Data</th>

<th>Timestamp</th>

<th>Action</th>

</tr>



<?php foreach($messages as $row){ ?>


<tr>


<td>
<?= $row["MessageID"] ?>
</td>


<td>
<?= $row["CANID"] ?>
</td>


<td>
<?= htmlspecialchars($row["MessageData"]) ?>
</td>


<td>
<?= $row["Timestamp"] ?>
</td>


<td>


<form method="post">


<input type="hidden" name="messageID" value="<?= $row["MessageID"] ?>">


<button name="deleteMessage"class="btn btn-danger">Delete</button>

</form>
</td>
</tr>


<?php } ?>


</table>



</body>

</html>