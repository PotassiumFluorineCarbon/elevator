// Includes required (headers located in /usr/include) 
#include "../include/databaseFunctions.h"
#include <stdlib.h>
#include <iostream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
 
using namespace std; 
 
int db_getFloorNum() {
	sql::Driver *driver; 			// Create a pointer to a MySQL driver object
	sql::Connection *con; 			// Create a pointer to a database connection object
	sql::Statement *stmt;			// Crealte a pointer to a Statement object to hold statements 
	sql::ResultSet *res;			// Create a pointer to a ResultSet object to hold results 
	int floorNum;					// Floor number 
	
	// Create a connection 
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "elevator_user", "password");	
	con->setSchema("elevator");		
	
	// Query database
	// ***************************** 
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT CurrentFloor FROM ElevatorStatus");	// message query
	while(res->next()){
		floorNum = res->getInt("CurrentFloor");
	}
	
	// Clean up pointers 
	delete res;
	delete stmt;
	delete con;
	
	return floorNum;
}
 
 
int db_setFloorNum(int floorNum) {
	sql::Driver *driver; 				// Create a pointer to a MySQL driver object
	sql::Connection *con; 				// Create a pointer to a database connection object
	sql::Statement *stmt;				// Crealte a pointer to a Statement object to hold statements 
	sql::ResultSet *res;				// Create a pointer to a ResultSet object to hold results 
	sql::PreparedStatement *pstmt; 		// Create a pointer to a prepared statement	
	
	// Create a connection 
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "elevator_user", "password");	
	con->setSchema("elevator");										
	
	// Query database (possibly not necessary)
	// ***************************** 
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT CurrentFloor FROM ElevatorStatus");	// message query
	while(res->next()){
		res->getInt("CurrentFloor");
	}
		
	// Update database
	// *****************************
	pstmt = con->prepareStatement("UPDATE ElevatorStatus SET CurrentFloor = ?");
	pstmt->setInt(1, floorNum);
	pstmt->executeUpdate();
		
	// Clean up pointers 
	delete res;
	delete pstmt;
	delete stmt;
	delete con;
} 

bool db_getNextCommand(int &canID, int &data) {

    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;
    sql::ResultSet *res;

    int commandID = -1;

    // Create connection
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "elevator_user", "password");
    con->setSchema("elevator");

    // ---------------------------------------------------------
    // First check for highest priority command (CANID=300 Data=2)
    // ---------------------------------------------------------
    pstmt = con->prepareStatement(
        "SELECT CommandID, CANID, Data "
        "FROM ElevatorCommands "
        "WHERE Status='pending' AND CANID=300 AND Data=2 "
        "LIMIT 1"
    );

    res = pstmt->executeQuery();

    if (!res->next()) {

        delete res;
        delete pstmt;

        // ---------------------------------------------------------
        // Otherwise get the oldest pending command
        // ---------------------------------------------------------
        pstmt = con->prepareStatement(
            "SELECT CommandID, CANID, Data "
            "FROM ElevatorCommands "
            "WHERE Status='pending' "
            "ORDER BY Timestamp ASC "
            "LIMIT 1"
        );

        res = pstmt->executeQuery();

        if (!res->next()) {
            delete res;
            delete pstmt;
            delete con;
            return false;
        }
    }

    commandID = res->getInt("CommandID");
    canID     = res->getInt("CANID");
    data      = res->getInt("Data");

    delete res;
    delete pstmt;

    // ---------------------------------------------------------
    // Mark command as complete
    // ---------------------------------------------------------
    pstmt = con->prepareStatement(
        "UPDATE ElevatorCommands "
        "SET Status='complete' "
        "WHERE CommandID=?"
    );

    pstmt->setInt(1, commandID);
    pstmt->executeUpdate();

    delete pstmt;
    delete con;

    return true;
}

int db_addCANMessage(int canID, int data) {

    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "elevator_user", "password");
    con->setSchema("elevator");

    pstmt = con->prepareStatement(
        "INSERT INTO CAN_messages (CANID, MessageData) VALUES (?, ?)"
    );

    pstmt->setInt(1, canID);
    pstmt->setInt(2, data);

    pstmt->executeUpdate();

    delete pstmt;
    delete con;

    return 0;
}

int db_addDiagnostic(int nodeID, const std::string &message) {

    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *pstmt;

    // Create connection
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "elevator_user", "password");
    con->setSchema("elevator");

    // Insert diagnostic message
    pstmt = con->prepareStatement(
        "INSERT INTO Diagnostics (NodeID, Message) VALUES (?, ?)"
    );

    pstmt->setInt(1, nodeID);
    pstmt->setString(2, message);

    pstmt->executeUpdate();

    // Clean up
    delete pstmt;
    delete con;

    return 0;
}
 
