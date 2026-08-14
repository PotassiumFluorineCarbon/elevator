<?php
// Database credentials
$path = "mysql:host=localhost;dbname=elevator";
$user = "elevator_user";
$password = "password";

try 
{
    $db = new PDO($path, $user, $password);
    $db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);
} catch (PDOException $e) 
{
    die("Database connection failed.");
}

// 1. Fetch latest elevator status
$statusQuery = $db->query("SELECT CurrentFloor, Direction, Timestamp FROM ElevatorStatus ORDER BY Timestamp DESC LIMIT 1");
$currentStatus = $statusQuery->fetch();

// 2. Fetch queued requests (pending commands)
$queueQuery = $db->query("SELECT CommandID, CANID, Data, Timestamp FROM ElevatorCommands WHERE Status = 'pending' ORDER BY Timestamp ASC");
$queuedRequests = $queueQuery->fetchAll();
$queueCount = count($queuedRequests);

// 3. Odometer: Count total commands processed today
$odometerQuery = $db->query("SELECT COUNT(*) as Total FROM ElevatorCommands WHERE DATE(Timestamp) = CURDATE()");
$odometer = $odometerQuery->fetch();
$commandsToday = $odometer['Total'];

// 4. Fetch Network Health
$healthQuery = $db->query("SELECT NodeName, Status FROM elevatorNetwork");
$networkNodes = $healthQuery ? $healthQuery->fetchAll() : [];

// Fallbacks in case the database is empty
$currentFloor = $currentStatus ? htmlspecialchars($currentStatus['CurrentFloor']) : 'Unknown';
$direction = $currentStatus ? htmlspecialchars($currentStatus['Direction']) : 'Idle';
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Elevator Live Status</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
        body { background-color: #f8f9fa; }
        .status-card { text-align: center; margin-bottom: 20px; border-radius: 12px; border: none; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
        .val-text { font-size: 2.5rem; font-weight: bold; }
        .queue-item { font-size: 1.1rem; }
        .text-idle { color: #6c757d; }
        .text-up { color: #198754; }
        .text-down { color: #dc3545; }
    </style>
</head>
<body>

<div class="container py-4">
    
    <div class="text-center mb-4">
        <h2 class="fw-bold">Live Elevator Status</h2>
        <p class="text-muted">Updated in real-time</p>
    </div>

    <div class="row">
        <!-- current floor -->
        <div class="col-6">
            <div class="card status-card p-3">
                <div class="card-title text-muted text-uppercase mb-1">Current Floor</div>
                <div class="val-text"><?= $currentFloor ?></div>
            </div>
        </div>

        <!-- odometer -->
        <div class="col-6">
            <div class="card status-card p-3 bg-primary text-white">
                <div class="card-title text-uppercase mb-1">Daily Operations</div>
                <div class="val-text mt-2"><?= $commandsToday ?></div>
            </div>
        </div>
    </div>

    <!-- direction -->
    <div class="card status-card p-3">
        <div class="card-title text-muted text-uppercase mb-1">Direction of Travel</div>
        <div class="val-text">
            <?php 
                if (strtolower($direction) === 'up') 
                {
                    echo '<span class="text-up">&#9650; UP</span>';
                } elseif (strtolower($direction) === 'down') 
                {
                    echo '<span class="text-down">&#9660; DOWN</span>';
                } else 
                {
                    echo '<span class="text-idle">&#9632; IDLE</span>';
                }
            ?>
        </div>
    </div>

    <!-- queued requests -->
    <div class="card shadow-sm border-0 mt-4">
        <div class="card-header bg-dark text-white">
            <h5 class="mb-0">Queued Requests (<?= $queueCount ?>)</h5>
        </div>
        <ul class="list-group list-group-flush">
            <?php if ($queueCount === 0): ?>
                <li class="list-group-item text-muted text-center py-3">No pending requests.</li>
            <?php else: ?>
                <?php foreach ($queuedRequests as $index => $req): ?>
                    <li class="list-group-item queue-item d-flex justify-content-between align-items-center">
                        <span>
                            <strong>CAN ID:</strong> <?= htmlspecialchars($req['CANID']) ?> <br>
                            <small class="text-muted">Data: <?= htmlspecialchars($req['Data']) ?></small>
                        </span>
                        <span class="badge bg-secondary rounded-pill">Pos #<?= $index + 1 ?></span>
                    </li>
                <?php endforeach; ?>
            <?php endif; ?>
        </ul>
    </div>

    <!-- System Health Card -->
    <div class="card shadow-sm border-0 mt-4">
        <div class="card-header bg-secondary text-white">
            <h5 class="mb-0">CAN Bus Network Health</h5>
        </div>
        <ul class="list-group list-group-flush">
            <?php if (empty($networkNodes)): ?>
                <li class="list-group-item text-muted text-center py-3">No network nodes found.</li>
            <?php else: ?>
                <?php foreach ($networkNodes as $node): ?>
                    <li class="list-group-item d-flex justify-content-between align-items-center">
                        <strong><?= htmlspecialchars($node['NodeName']) ?></strong>
                        <?php if (strtolower($node['Status']) === 'online' || strtolower($node['Status']) === 'active'): ?>
                            <span class="badge bg-success rounded-pill">ONLINE</span>
                        <?php else: ?>
                            <span class="badge bg-danger rounded-pill"><?= htmlspecialchars($node['Status']) ?></span>
                        <?php endif; ?>
                    </li>
                <?php endforeach; ?>
            <?php endif; ?>
        </ul>
    </div>
    
    <!-- Auto-refresh script -->
    <script>
        // Automatically refresh the page every 1.5 seconds (1500 milliseconds)
        setInterval(function() {
            window.location.reload();
        }, 1500);
    </script>

</div>

</body>
</html>
