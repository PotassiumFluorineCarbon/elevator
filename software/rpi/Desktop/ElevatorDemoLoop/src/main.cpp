#include "../include/pcanFunctions.h"
#include "../include/databaseFunctions.h"
#include "../include/mainFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <iostream>

using namespace std;


// ******************************************************************

void elevatoroperator() {
	enum State state = initial;
	TPCANMsg Rxmsg;
	HANDLE h2;

	// Open a CAN channel 
	h2 = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h2, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h2);

	while (1) {
		while ((status = CAN_Read(h2, &Rxmsg)) == PCAN_RECEIVE_QUEUE_EMPTY) {//read message from CAN bus
			// No message received, continue waiting

		}

		switch (state) {
		case initial:
			// Initialize elevator
			if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR1) {
				state = waiting_at_1;
			}
			else if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR2) {
				state = waiting_at_2;
			}
			else if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR3) {
				state = waiting_at_3;
			}
			break;
		case waiting_at_1:
			// Waiting at floor 1
			// If command received, state = moving_to_2 or 3
			if (Rxmsg.ID == 0x202 && Rxmsg.DATA[0] == 0x01) {  // FC2 is calling
				// now send message to EC to MOVE TO FLOOR 2: CAN ID 0X100, MESSAGE BYTE 0X06
				sendMsg(0x100, 0x06, h2);
				state = moving_to_2;
			}
			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x02) {
				sendMsg(0x100, 0x06, h2)
					state = moving_to_2;
			}

			if (Rxmsg.ID == 0x203 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x07, h2);
				state = moving_to_3;
			}

			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x03) {
				sendMsg(0x100, 0x07, h2);
				state = moving_to_3;
			}
			break;
		case moving_to_1:
			if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR1) {
				state = waiting_at_1;
			}

			break;
		case waiting_at_2:
			if (Rxmsg.ID == 0x201 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2);
				state = moving_to_1;
			}
			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2)
					state = moving_to_1;
			}

			if (Rxmsg.ID == 0x203 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x07, h2);
				state = moving_to_3;
			}

			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x03) {
				sendMsg(0x100, 0x07, h2);
				state = moving_to_3;
			}
			break;
		case moving_to_2:
			if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR2) {
				state = waiting_at_2;
			}

			break;
		case waiting_at_3:
			if (Rxmsg.ID == 0x201 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2);
				state = moving_to_1;
			}
			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2)
					state = moving_to_1;
			}

			if (Rxmsg.ID == 0x202 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x06, h2);
				state = moving_to_2;
			}

			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x02) {
				sendMsg(0x100, 0x06, h2);
				state = moving_to_2;
			}

			break;
		case moving_to_3:
			if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR3) {
				state = waiting_at_3;
			}

			break;
		case fault:
			// Handle fault condition (e.g. stop elevator, sound alarm, etc.)
			break;
		}
		printf("\nState: ", state);
	}
}

int main() {

	int choice; 
	int ID; 
	int data; 
	int numRx;
	int floorNumber = 1, prev_floorNumber = 1;

	while(1) {
		system("@cls||clear");
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
				printf("Elevator operator");
				elevatoroperator();
			case 6:
				return(0);
			default:
				printf("Error on input values");
				sleep(3);
				break;
		}
		sleep(1);					// delay between send/receive
	}
	
	return(0);
}






	
