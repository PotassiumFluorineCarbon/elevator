<?php

require_once "databaseFunctions.php";

$path="mysql:host=localhost;dbname=elevator";
$user="elevator_user";
$password="password";

$db=connect($path,$user,$password);

$status=$db->query(
"SELECT * FROM ElevatorStatus ORDER BY Timestamp DESC LIMIT 1"
)->fetch();

?>

<h3>Elevator Status</h3>

<p><b>Current Floor:</b> <?= htmlspecialchars($status["CurrentFloor"]) ?></p>

<p><b>Direction:</b> <?= htmlspecialchars($status["Direction"]) ?></p>

<p><b>Last Update:</b> <?= htmlspecialchars($status["Timestamp"]) ?></p>

<div class="shaft">
<div class="floor <?php if($status["CurrentFloor"]==3) echo "current"; ?>">
Floor 3
</div>

<div class="floor <?php if($status["CurrentFloor"]==2) echo "current"; ?>">
Floor 2
</div>

<div class="floor <?php if($status["CurrentFloor"]==1) echo "current"; ?>">
Floor 1
</div>
</div>