#include "../include/pcanFunctions.h"
#include "../include/databaseFunctions.h"
#include "../include/mainFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <iostream>
#include <system_error>

using namespace std;

// ******************************************************************
static void init_can_interface(void) {
	printf("Bringing up can0 interface...\n");
	system("sudo ip link set can0 type can bitrate 125000");
	system("sudo ip link set can0 up");
	sleep(1);
}

int main() {
	init_can_interface();

	int choice; 
	int ID; 
	int data; 
	int numRx;
	int floorNumber = 1, prev_floorNumber = 1;
	
	while(1) {
		try {
		//system("@cls||clear");
		choice = menu(); 
		switch (choice) {
			case 1: 
				ID = chooseID();		// user to select ID depending on intended recipient
				data = chooseMsg();		// user to select message data
				pcanTx(ID, data);		// transmit ID and data 
				db_setFloorNum(FloorFromHex(data)); 		// change floor number in database ** NEW **
				break; 
				
			case 2:
				printf("\nHow many messages to receive? ");
				scanf("%d", &numRx);
				pcanRx(numRx);
				break;
				
			case 3:
				printf("\nNow listening to commands from the website - press ctrl-z to cancel\n");
				// Synchronize elevator db and CAN (start at 1st floor)
				pcanTx(ID_SC_TO_EC, GO_TO_FLOOR1);
				db_setFloorNum(1);
				
				while(1){			
					floorNumber = db_getFloorNum();
					if (prev_floorNumber != floorNumber) {								// If floor number changes in database
						pcanTx(ID_SC_TO_EC, HexFromFloor(floorNumber));					// change floor number in elevator - send command over CAN
					}
					prev_floorNumber = floorNumber; 
					sleep(1);															// poll database once every second to check for change in floor number
				}
				break;
				
			case 4:
				
				printf("\nDemo Mode - loop from floor to floor - press ctrl-z to cancel\n");
				while(1) {
					pcanTx(ID_SC_TO_EC, GO_TO_FLOOR1);
					db_setFloorNum(1);
					sleep(20);
					pcanTx(ID_SC_TO_EC, GO_TO_FLOOR2);
					db_setFloorNum(2);
					sleep(20);
					pcanTx(ID_SC_TO_EC, GO_TO_FLOOR3);
					db_setFloorNum(3);
					sleep(20);
				}
				break;
				
			case 5: 
				printf("Elevator operator\n");
				elevatoroperator();
				break;
			case 6:
				return(0);
			default:
				printf("Error on input values\n");
				sleep(3);
				break;
		}
		sleep(1);					// delay between send/receive
	
	} catch (const std::system_error& e) {
		printf("CAN error: %s\n", e.what());
		return 1;
	}
}
	return(0);
}






	
