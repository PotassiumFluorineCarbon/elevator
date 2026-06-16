#include "../include/pcanFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>  
#include <errno.h>
#include <unistd.h> 
#include <signal.h>
#include <string.h>
#include <fcntl.h>    					// O_RDWR
#include <unistd.h>
#include <ctype.h>
#include <libpcan.h>   					// PCAN library


// Globals
// ***********************************************************************************************************
HANDLE h;
HANDLE h2;
TPCANMsg Txmsg;
TPCANMsg Rxmsg;
DWORD status;

// Code
// ***********************************************************************************************************
int sendMsg(int id, int data, HANDLE h);

enum State {
	initial,
	waiting_at_1,
	moving_to_1,
	waiting_at_2,
	moving_to_2,
	waiting_at_3,
	moving_to_3,
	fault
};
// Functions
// *****************************************************************
int pcanTx(int id, int data){
	h = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);		// Open PCAN channel

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h);

	// Set up message
	Txmsg.ID = id; 	
	Txmsg.MSGTYPE = MSGTYPE_STANDARD; 
	Txmsg.LEN = 1; 
	Txmsg.DATA[0] = data; 

	sleep(1);  
	status = CAN_Write(h, &Txmsg);
	// Close CAN 2.0 channel and exit	

	if (status != PCAN_NO_ERROR) {
		printf("pcanTx CAN Write failed: 0x%x\n", (int)status);
	}
	else {
		printf("pcanTx [Tx] ID: 0x%04X  DATA: 0x%02X  --> Sent!\n", id, data);
	}
	CAN_Close(h);
}

int pcanRx(int num_msgs){
	int i = 0;

	// Open a CAN channel 
	h2 = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h2, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h2);

	// Clear screen to show received messages
	system("@cls||clear");

	// receive CAN message  - CODE adapted from PCAN BASIC C++ examples pcanread.cpp
	printf("\nReady to receive message(s) over CAN bus\n");
	
	// Read 'num' messages on the CAN bus
	while(i < num_msgs) {
		while((status = CAN_Read(h2, &Rxmsg)) == PCAN_RECEIVE_QUEUE_EMPTY){
			sleep(1);
		}
		if(status != PCAN_NO_ERROR) {						// If there is an error, display the code
			printf("Error 0x%x\n", (int)status);
			//break;
		}
										
		if(Rxmsg.ID != 0x01 && Rxmsg.LEN != 0x04) {		// Ignore status message on bus	
			printf("  - R ID:%4x LEN:%1x DATA:%02x \n",	// Display the CAN message
				(int)Rxmsg.ID, 
				(int)Rxmsg.LEN,
				(int)Rxmsg.DATA[0]);
		i++;
		}
	}
	

	// Close CAN 2.0 channel and exit	
	CAN_Close(h2);
	//printf("\nEnd Rx\n");
	return ((int)Rxmsg.DATA[0]);						// Return the last value received
}

void elevatoroperator() {
	enum State state = initial;
	TPCANMsg Rxmsg = {0};
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
			usleep(50000);
		}
		printf("Rxmsg ID: %x\n", Rxmsg.ID);
		printf("Rxmsg data: %x\n", Rxmsg.DATA[0]);

		switch (state) {
		case initial:
			printf("Initial\n");
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
			printf("Waiting at 1\n");
			// Waiting at floor 1
			// If command received, state = moving_to_2 or 3
			if (Rxmsg.ID == 0x202 && Rxmsg.DATA[0] == 0x01) {  // FC2 is calling
				// now send message to EC to MOVE TO FLOOR 2: CAN ID 0X100, MESSAGE BYTE 0X06
				sendMsg(0x100, 0x06, h2);
				state = moving_to_2;
			}
			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x02) {
				sendMsg(0x100, 0x06, h2);
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
			printf("Moving to 1\n");
			if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR1) {
				state = waiting_at_1;
			}

			break;
		case waiting_at_2:
			printf("Waiting at 2\n");
			if (Rxmsg.ID == 0x201 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2);
				state = moving_to_1;
			}
			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2);
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
			printf("Moving to 2\n");
			if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR2) {
				state = waiting_at_2;
			}

			break;
		case waiting_at_3:
			printf("Waiting at 3\n");
			if (Rxmsg.ID == 0x201 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2);
				state = moving_to_1;
			}
			if (Rxmsg.ID == 0x200 && Rxmsg.DATA[0] == 0x01) {
				sendMsg(0x100, 0x05, h2);
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
			printf("Moving to 3\n");
			if (Rxmsg.ID == ID_EC_TO_ALL && Rxmsg.DATA[0] == AT_FLOOR3) {
				state = waiting_at_3;
			}

			break;
		case fault:
			printf("Error\n");
			// Handle fault condition (e.g. stop elevator, sound alarm, etc.)
			break;
		}
	}
}



int sendMsg(int id, int data, HANDLE h) {
	//printf("\nTxmsg id: %d", id);
	//printf("\nTxmsg data: %d", data);
	TPCANMsg Txmsg = {0};
	// Set up message
	Txmsg.ID = id;
	Txmsg.MSGTYPE = MSGTYPE_STANDARD;
	Txmsg.LEN = 1;
	Txmsg.DATA[0] = data;

	status = CAN_Write(h, &Txmsg);

	if (status != PCAN_NO_ERROR) {
		printf("sendMsg CAN Write failed: 0x%x\n", (int)status);
	}
	else {
		printf("sendMsg [Tx] ID: 0x%04X  DATA: 0x%02X  --> Sent!\n", id, data);
	}
	return status;
}

