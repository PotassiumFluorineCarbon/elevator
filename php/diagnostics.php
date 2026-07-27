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

if(isset($_POST["deleteDiagnostic"]))
{
    clearDiagnostic(
        $path,
        $user,
        $password,
        $_POST["diagnosticID"]
    );

    header("Location: diagnostics.php");
    exit();
}

$db = connect($path,$user,$password);

$diagnostics = $db->query(
    "SELECT * FROM Diagnostics ORDER BY Timestamp DESC"
);
?>

<!DOCTYPE html>
<html>

<head>

<meta charset="utf-8">

<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">

<title>Diagnostics</title>

<link rel="stylesheet" href="../CSS/style.css">

</head>

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
<a class="nav-link active bg-dark text-white" href="diagnostics.php">
Diagnostics
</a>
</li>

<li class="nav-item">
<a class="nav-link text-dark" href="messages.php">
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

<body class="control-panel">

<h2>
Welcome
<?= htmlspecialchars($_SESSION["username"]) ?>
</h2>

<h4>Diagnostics</h4>

<table class="command-table">

<tr>

<th>ID</th>

<th>Node ID</th>

<th>Timestamp</th>

<th>Message</th>

<th>Action</th>

</tr>

<?php foreach($diagnostics as $row){ ?>

<tr>

<td><?= $row["DiagnosticID"] ?></td>

<td><?= $row["NodeID"] ?></td>

<td><?= $row["Timestamp"] ?></td>

<td><?= htmlspecialchars($row["Message"]) ?></td>

<td>

<form method="post">

<input
type="hidden"
name="diagnosticID"
value="<?= $row["DiagnosticID"] ?>"
>

<button
name="deleteDiagnostic"
class="btn btn-danger"
>

Delete

</button>

</form>

</td>

</tr>

<?php } ?>

</table>

</body>

</html>