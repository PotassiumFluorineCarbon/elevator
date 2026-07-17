<?php 

// CRUD (Create, Read, Update, Delete) functions


// Connect
function connect(string $path, string $user, string $password) {

    $db = new PDO($path, $user, $password);

    $db->setAttribute(
        PDO::ATTR_DEFAULT_FETCH_MODE,
        PDO::FETCH_ASSOC
    );

    return $db; 
}



// Create
function insert(
    string $path,
    string $user,
    string $password,
    string $NodeName,
    string $nodeType,
    string $NetworkType,
    string $Status
) : void {


    $db = connect($path, $user, $password);


    $query = 
    'INSERT INTO elevatorNetwork
    (NodeName, nodeType, NetworkType, Status)

    VALUES

    (:NodeName, :nodeType, :NetworkType, :Status)';



    $statement = $db->prepare($query);


    $statement->bindValue(
        'NodeName',
        $NodeName
    );


    $statement->bindValue(
        'nodeType',
        $nodeType
    );


    $statement->bindValue(
        'NetworkType',
        $NetworkType
    );


    $statement->bindValue(
        'Status',
        $Status
    );


    $statement->execute();

}




// Read
function showtable(
    string $path,
    string $user,
    string $password,
    string $tablename
) : void {


    $db = connect($path, $user, $password);


    $query = 
    "SELECT * FROM $tablename ORDER BY nodeID";


    $rows = $db->query($query);



    echo "<table border='1'>";

    echo "
    <tr>
    <th>Node ID</th>
    <th>Node Name</th>
    <th>Node Type</th>
    <th>Network Type</th>
    <th>Status</th>
    </tr>
    ";



    foreach($rows as $row)
    {

        echo "<tr>";

        echo "<td>".$row['nodeID']."</td>";

        echo "<td>".$row['NodeName']."</td>";

        echo "<td>".$row['nodeType']."</td>";

        echo "<td>".$row['NetworkType']."</td>";

        echo "<td>".$row['Status']."</td>";

        echo "</tr>";

    }


    echo "</table>";

}




// Update
function update(
    string $path,
    string $user,
    string $password,
    string $tablename,
    int $node_ID,
    string $new_NodeName,
    string $new_nodeType,
    string $new_NetworkType,
    string $new_Status
) : void {


    $db = connect($path, $user, $password);



    $query = 

    'UPDATE ' . $tablename . '

    SET

    NodeName = :NodeName,

    nodeType = :nodeType,

    NetworkType = :NetworkType,

    Status = :Status


    WHERE nodeID = :id';



    $statement = $db->prepare($query);



    $statement->bindValue(
        'NodeName',
        $new_NodeName
    );


    $statement->bindValue(
        'nodeType',
        $new_nodeType
    );


    $statement->bindValue(
        'NetworkType',
        $new_NetworkType
    );


    $statement->bindValue(
        'Status',
        $new_Status
    );


    $statement->bindValue(
        'id',
        $node_ID
    );



    $statement->execute();

}





// Delete
function delete(
    string $path,
    string $user,
    string $password,
    string $tablename,
    int $node_ID
) : void {


    $db = connect($path, $user, $password);



    $query =
    'DELETE FROM ' . $tablename .
    ' WHERE nodeID = :id';



    $statement = $db->prepare($query);



    $statement->bindValue(
        'id',
        $node_ID
    );



    $statement->execute();

}

function updateCommand(
    string $path,
    string $user,
    string $password,
    int $canid,
    int $data
)
{
    $db = connect($path,$user,$password);

    $query = "

    UPDATE ElevatorCommands

    SET

    Timestamp = NOW(),
    Status='pending'

    WHERE CANID=:canid
    AND Data=:data

    ";

    $statement = $db->prepare($query);

    $statement->bindValue("canid",$canid);
    $statement->bindValue("data",$data);

    $statement->execute();
}


?>