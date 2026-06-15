#include "../include/pcanFunctions.h"
#include "../include/mainFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h> 
#include <array>

int menu(){
	
	int usrchoice = 0;
	system("@cls||clear");
	while(1) {
		printf("\n\nMenu - Transmit/Receive CAN Messages\n");
		printf("1. Transmit CAN message using this program\n");
		printf("2. Receive CAN message(s) using this program\n");
		printf("3. Control elevator from website\n");
		printf("4. Demo mode - loop\n");
		printf("5. Exit program\n");
		printf("\nYour choice: ");
		scanf("%d", &usrchoice);

		if (usrchoice >=1 && usrchoice <= 5) {	
			return usrchoice;
		} else {
			printf("\nPLEASE SELECT FROM CHOICES 1-5 ONLY!\n\n");
			sleep(3);
			system("@cls||clear");
		}
	}
	
}


int chooseID(){

	int IdChoice = 0;		// Menu item number
	int IDvalue = 0 ;		// ID value in HEX
	while(1) {
		system("@cls||clear");
		printf("\nChoose sender and receiver for message\n");
		printf("1. Message from Supervisory controller (i.e. this node) to Elecvator Controller\n");
		printf("2. Message from Elevator controller to all other nodes\n");
		printf("3. Message from Car controller to supervisory controller (this node)\n");
		printf("4. Message from floor 1 controller to supervisory controller (this node)\n");
		printf("5. Message from floor 2 controller to supervisory controller (this node)\n");
		printf("6. Message from floor 3 controller to supervisory controller (this node)\n");

		printf("\nYour choice: ");
		scanf("%d", &IdChoice);

		if (IdChoice >=1 && IdChoice <= 6) {	
			switch(IdChoice) {
				case 1:
					IDvalue = ID_SC_TO_EC; 
					return(IDvalue);
				case 2:
					IDvalue = ID_EC_TO_ALL; 
					return(IDvalue);
				case 3:
					IDvalue = ID_CC_TO_SC; 
					return(IDvalue);
				case 4:
					IDvalue = ID_F1_TO_SC; 
					return(IDvalue);
				case 5:
					IDvalue = ID_F2_TO_SC; 
					return(IDvalue);
				case 6:
					IDvalue = ID_F3_TO_SC; 
					return(IDvalue);
			}

		} else {
			printf("\nPLEASE SELECT FROM CHOICES 1-6 ONLY!\n\n");
			sleep(3);
		}

	}
}

int chooseMsg(){
	int messageChoice = 0; 
	int messageValue = 0;
	
	while(1) {
		system("@cls||clear");
		printf("\nChoose Message\n");
		printf("1. Go to floor 1\n");
		printf("2. Go to floor 2\n");
		printf("3. Go to floor 3\n");
		printf("\nYour choice: ");
		scanf("%d", &messageChoice);

		if (messageChoice >=1 && messageChoice <= 3) {	
			switch(messageChoice) {
				case 1:
					messageValue = GO_TO_FLOOR1; 
					return(messageValue);
					break;
				case 2:
					messageValue = GO_TO_FLOOR2; 
					return(messageValue);
					break;
				case 3:
					messageValue = GO_TO_FLOOR3; 
					return(messageValue);
					break;
			}

		} else {
			printf("PLEASE SELECT FROM CHOICES 1-3 ONLY!\n\n");
			sleep(3);
		}
	}
}


int HexFromFloor(int floorVal) {

	switch(floorVal) {
		case 1:
			return(GO_TO_FLOOR1);
			break;
		case 2:
			return(GO_TO_FLOOR2);
			break;
		case 3: 
			return(GO_TO_FLOOR3);
			break;
		default:
			return(GO_TO_FLOOR1);			// Default is to reset to floor 1 on bad input
		}
}

int FloorFromHex(int Hex){
		
	switch(Hex) {
		case GO_TO_FLOOR1:
			return(1);
			break;
		case GO_TO_FLOOR2:
			return(2);
			break;
		case GO_TO_FLOOR3:
			return(3);
			break;
		default:
			return(1);							// Default is to reset to floor 1 on bad input
		}
}



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


int sendMsg(int id,int data,HANDLE h){
	TPCANMsg Txmsg;
	// Set up message
	Txmsg.ID = id;
	Txmsg.MSGTYPE = MSGTYPE_STANDARD;
	Txmsg.LEN = 1;
	Txmsg.DATA[0] = data;

	status = CAN_Write(h, &Txmsg);
	printf("\nTxmsg: ", txmsg);
}

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