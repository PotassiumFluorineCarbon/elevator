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

function press($name,$can,$data,$path,$user,$password){
    if(isset($_POST[$name])){
        updateCommand($path,$user,$password,$can,$data);
        header("Location: controlPanel.php");
        exit();
    }
}

press("doorOpen",10,0,$path,$user,$password);
press("doorClose",10,1,$path,$user,$password);
press("car1",200,1,$path,$user,$password);
press("car2",200,2,$path,$user,$password);
press("car3",200,3,$path,$user,$password);
press("f1up",201,4,$path,$user,$password);
press("f2down",202,2,$path,$user,$password);
press("f2up",202,4,$path,$user,$password);
press("f3down",203,2,$path,$user,$password);
clear($path,$user,$password);

function clear($path,$user,$password){
    if(isset($_POST["clearCommands"])){
        clearCommands($path,$user,$password);
        header("Location: controlPanel.php");
        exit();
    }
}

$db=connect($path,$user,$password);
$status=$db->query("SELECT * FROM ElevatorStatus ORDER BY Timestamp DESC LIMIT 1")->fetch();
$commands=$db->query("SELECT * FROM ElevatorCommands ORDER BY Timestamp DESC");
?>
<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <title>Control Panel</title>
    <style>
        body{font-family:Arial;background:#eee;margin:30px}
        .panel{display:flex;gap:40px}
        .left,.right{background:#fff;padding:20px;border-radius:8px}
        button{width:180px;height:42px;margin:4px;font-size:15px}
        table{border-collapse:collapse;margin-top:20px;background:#fff}
        td,th{border:1px solid #555;padding:6px 10px}
        .shaft{width:120px;border:2px solid #000}
        .floor{height:70px;border-top:1px solid #888;display:flex;align-items:center;justify-content:center}
        .current{background:#8f8;font-weight:bold}
    </style>
</head>

<header class="bg-dark text-white py-4 mb-3">
    <div class="container text-center">
        <h1 class="display-5 fw-bold">Elevator Control System</h1>
    </div>
</header>

<nav class="container mb-4">
    <ul class="nav nav-pills justify-content-center bg-white py-2 rounded shadow">

        <li class="nav-item">
            <a class="nav-link active bg-dark text-white" href="controlPanel.php">
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

<body>
    <h2>Welcome <?php echo htmlspecialchars($_SESSION['username']); ?></h2>
    <p><a href="logout.php">Logout</a></p>

    <div class="panel">
    <div class="left" id="status">
    <h3>Elevator Status</h3>
        <p><b>Current Floor:</b> <?= htmlspecialchars($status["CurrentFloor"]) ?></p>
        <p><b>Direction:</b> <?= htmlspecialchars($status["Direction"]) ?></p>
        <p><b>Last Update:</b> <?= htmlspecialchars($status["Timestamp"]) ?></p>

<div class="shaft">
    <div class="floor <?php if($status["CurrentFloor"]==3) echo "current"; ?>">Floor 3</div>
    <div class="floor <?php if($status["CurrentFloor"]==2) echo "current"; ?>">Floor 2</div>
    <div class="floor <?php if($status["CurrentFloor"]==1) echo "current"; ?>">Floor 1</div>
</div>
</div>

<div class="right">
<form method="post">
<h4>Car Calls</h4>
<button name="car1">Go To Floor 1</button><br>
<button name="car2">Go To Floor 2</button><br>
<button name="car3">Go To Floor 3</button>

<h4>Floor Calls</h4>
<button name="f1up">Floor 1 Up</button><br>
<button name="f2up">Floor 2 Up</button><br>
<button name="f2down">Floor 2 Down</button><br>
<button name="f3down">Floor 3 Down</button>

<h4>Clear</h4>
<button name="clearCommands">Clear</button>

</form>



</div>
</div>

<h3>Command Status</h3>
<table>
    <tr><th>ID</th><th>CANID</th><th>Data</th><th>Timestamp</th><th>Status</th></tr>
    <?php foreach($commands as $row){ ?>
        <tr>
        <td><?= $row["CommandID"] ?></td>
        <td><?= $row["CANID"] ?></td>
        <td><?= $row["Data"] ?></td>
        <td><?= $row["Timestamp"] ?></td>
        <td><?= $row["Status"] ?></td>
        </tr>
    <?php } ?>
</table>

<script>
setInterval(function(){
    fetch("getStatus.php")
    .then(response => response.text())
    .then(data => {
        document.getElementById("status").innerHTML = data;
    });
},1000);
</script>

</body>
</html>
