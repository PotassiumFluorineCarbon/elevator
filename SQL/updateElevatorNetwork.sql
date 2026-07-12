USE elevator;

DELIMITER $$

CREATE PROCEDURE UpdateElevatorNetworkField(
    IN input_nodeID INT,
    IN input_field VARCHAR(50),
    IN input_value VARCHAR(100)
)

BEGIN

    -- Prevent changing the primary key
    IF input_field = 'nodeID' THEN

        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Cannot update primary key';

    END IF;


    -- Only allow valid columns
    IF input_field NOT IN
    (
        'NodeName',
        'nodeType',
        'NetworkType',
        'Status'
    )

    THEN

        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid column name';

    END IF;


    -- Dynamically update the selected field
    SET @query = CONCAT(
        'UPDATE elevatorNetwork SET ',
        input_field,
        ' = ? WHERE nodeID = ?'
    );


    PREPARE statement FROM @query;


    EXECUTE statement
    USING input_value, input_nodeID;


    DEALLOCATE PREPARE statement;


END$$

DELIMITER ;
