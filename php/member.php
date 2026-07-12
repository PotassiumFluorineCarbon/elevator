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

</head>



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