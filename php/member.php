<?php

error_reporting(E_ALL);
ini_set('display_errors', 1);

session_start();


if (!isset($_SESSION['username'])) {

    header("Location: ../html/login.html");

    exit();

}

require_once __DIR__ . "/databaseFunctions.php";
echo "Database functions loaded";



$path = "mysql:host=localhost;dbname=elevator";

$user = "elevator";

$password = "";




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



<body>



<p>
Welcome,
<?php echo htmlspecialchars($_SESSION['username']); ?>
</p>



<p>
<a href="logout.php">Logout</a>
</p>



<h1>Elevator Network</h1>



<?php

showtable(
    $path,
    $user,
    $password,
    "elevatorNetwork"
);

?>



<h2>Add Node</h2>


<form method="post">


Node Name:

<input name="NodeName">


<br><br>


Node Type:

<input name="nodeType">


<br><br>


Network Type:

<input name="NetworkType">


<br><br>


Status:

<input name="Status">


<br><br>


<button name="insert">

INSERT

</button>




</form>





<h2>Update/Delete Node</h2>


<form method="post">


Node ID:

<input name="nodeID">


<br><br>


Node Name:

<input name="NodeName">


<br><br>


Node Type:

<input name="nodeType">


<br><br>


Network Type:

<input name="NetworkType">


<br><br>


Status:

<input name="Status">


<br><br>


<button name="update">

UPDATE

</button>



<button name="delete">

DELETE

</button>


</form>



</body>

</html>