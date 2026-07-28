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

/**
 * Question 7: Function for updating ANY field in a table (except primary key)
 */
function updateField(
    string $path, 
    string $user, 
    string $password, 
    string $tablename, 
    int $nodeID, 
    string $fieldToUpdate, 
    $newValue
): void {
    // 1. Whitelist of allowed fields to prevent SQL Injection (excluding PK 'nodeID')
    $allowedFields = ['NodeName', 'nodeType', 'NetworkType', 'Status'];

    if (!in_array($fieldToUpdate, $allowedFields)) {
        throw new InvalidArgumentException("Invalid field name or primary key update is forbidden.");
    }

    $db = connect($path, $user, $password);

    // 2. Build query dynamically with the validated field name
    $query = "UPDATE $tablename SET $fieldToUpdate = :newValue WHERE nodeID = :id";
    
    $statement = $db->prepare($query);
    $statement->bindValue(':newValue', $newValue);
    $statement->bindValue(':id', $nodeID);
    
    $statement->execute();
}

/**
 * Question 8: Updated function using Transactions and Exception handling
 */
function updateFieldWithTransaction(
    string $path, 
    string $user, 
    string $password, 
    string $tablename, 
    int $nodeID, 
    string $fieldToUpdate, 
    $newValue
): void {
    // Whitelist check for safety
    $allowedFields = ['NodeName', 'nodeType', 'NetworkType', 'Status'];
    if (!in_array($fieldToUpdate, $allowedFields)) {
        throw new InvalidArgumentException("Error: '{$fieldToUpdate}' is an invalid column or primary key.");
    }

    $db = connect($path, $user, $password);

    try {
        // Begin Transaction
        $db->beginTransaction();

        $query = "UPDATE $tablename SET $fieldToUpdate = :newValue WHERE nodeID = :id";
        $statement = $db->prepare($query);
        $statement->bindValue(':newValue', $newValue);
        $statement->bindValue(':id', $nodeID);
        
        $statement->execute();

        // Check if any row was actually updated
        if ($statement->rowCount() === 0) {
            throw new Exception("Update failed or no row matched nodeID = {$nodeID}. Rolling back.");
        }

        // Commit Transaction
        $db->commit();
        echo "Update successful for nodeID {$nodeID}!<br>";

    } catch (Exception $e) {
        // Rollback on any failure/exception
        if ($db->inTransaction()) {
            $db->rollBack();
        }
        echo "Transaction Failed: " . $e->getMessage() . "<br>";
    }
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

function clearCommands(
    string $path,
    string $user,
    string $password
)
{
    $db = connect($path,$user,$password);

    $query = "
        UPDATE ElevatorCommands
        SET Status='complete'
        WHERE Status='pending'
    ";

    $statement = $db->prepare($query);
    $statement->execute();
}

function clearDiagnostic(
    string $path,
    string $user,
    string $password,
    int $diagnosticID
) : void
{
    $db = connect($path,$user,$password);

    $query = "
        DELETE FROM Diagnostics
        WHERE DiagnosticID = :id
    ";

    $statement = $db->prepare($query);

    $statement->bindValue("id",$diagnosticID);

    $statement->execute();
}

function clearMessages(
    string $path,
    string $user,
    string $password,
    int $messageID
) : void
{
    $db = connect($path,$user,$password);

    $query = "
        DELETE FROM CAN_messages
        WHERE MessageID = :id
    ";

    $statement = $db->prepare($query);

    $statement->bindValue("id",$messageID);

    $statement->execute();
}


?>