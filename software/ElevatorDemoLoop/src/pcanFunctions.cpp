#include "../include/pcanFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

static int can_socket = -1;
int sendMsg(int id, int data, int sock);

// Open / bind SocketCAN interface (can0)
static int can_open(void) {
	if (can_socket >= 0) return can_socket;

	struct sockaddr_can addr;
	struct ifreq ifr;

	can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (can_socket < 0) {
		perror("socket CAN");
		return -1;
	}

	strcpy(ifr.ifr_name, "can0");
	if (ioctl(can_socket, SIOCGIFINDEX, &ifr) < 0) {
		perror("ioctl SIOCGIFINDEX");
		close(can_socket);
		can_socket = -1;
		return -1;
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(can_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("bind CAN");
		close(can_socket);
		can_socket = -1;
		return -1;
	}

	// Non-blocking mode
	fcntl(can_socket, F_SETFL, O_NONBLOCK);

	printf("SocketCAN: can0 opened successfully\n");
	return can_socket;
}

static void can_close(void) {
	if (can_socket >= 0) {
		close(can_socket);
		can_socket = -1;
	}
}

int pcanTx(int id, int data) {
	if (can_open() < 0) return -1;

	struct can_frame frame = { 0 };
	frame.can_id = id;
	frame.can_dlc = 1;
	frame.data[0] = (unsigned char)data;

	ssize_t nbytes = write(can_socket, &frame, sizeof(frame));
	if (nbytes != sizeof(frame)) {
		perror("CAN write");
		printf("pcanTx FAILED: ID=0x%04X DATA=0x%02X\n", id, data);
		return -1;
	}

	printf("pcanTx [Tx] ID: 0x%04X  DATA: 0x%02X  --> Sent!\n", id, data);
	return 0;
}

int pcanRx(int num_msgs) {
	if (can_open() < 0) return -1;

	struct can_frame frame;
	int received = 0;

	system("@cls||clear");
	printf("\nReady to receive %d message(s) over CAN bus (can0)\n", num_msgs);

	while (received < num_msgs) {
		ssize_t nbytes = read(can_socket, &frame, sizeof(frame));

		if (nbytes < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				usleep(50000);
				continue;
			}
			perror("CAN read");
			break;
		}

		if (nbytes == sizeof(frame)) {
			// Filter out status messages if needed
			if (frame.can_id == 0x01 && frame.can_dlc == 4) {
				continue;
			}

			printf("  - R ID:0x%04X LEN:%d DATA:0x%02X\n",
				frame.can_id, frame.can_dlc, frame.data[0]);

			received++;
		}
	}
	return (int)frame.data[0];   // return last received data byte
}

// ==================== ELEVATOR STATE MACHINE ====================

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

void elevatoroperator() {
	if (can_open() < 0) {
		printf("Failed to open CAN socket!\n");
		return;
	}

	struct can_frame Rxmsg;
	enum State state = initial;

	printf("\nElevator Operator State Machine Started (SocketCAN)\n");
	printf("Press Ctrl+C to exit\n\n");

	while (1) {
		ssize_t nbytes = read(can_socket, &Rxmsg, sizeof(Rxmsg));

		if (nbytes == sizeof(Rxmsg)) {
			printf("Rx ID: 0x%X  Data: 0x%02X\n", Rxmsg.can_id, Rxmsg.data[0]);

			switch (state) {
			case initial:
				printf("Initial\n");//Initialize elevator
				if (Rxmsg.can_id == ID_EC_TO_ALL) {
					if (Rxmsg.data[0] == AT_FLOOR1) state = waiting_at_1;
					else if (Rxmsg.data[0] == AT_FLOOR2) state = waiting_at_2;
					else if (Rxmsg.data[0] == AT_FLOOR3) state = waiting_at_3;
				}
				break;

			case waiting_at_1:
				printf("Waiting at 1\n");
				// Waiting at floor 1
				// If command received, state = moving_to_2 or 3
				if (Rxmsg.can_id == ID_F2_TO_SC && Rxmsg.data[0] == 0x01) {  // FC2 is calling
					// now send message to EC to MOVE TO FLOOR 2: CAN ID 0X100, MESSAGE BYTE GO_TO_FLOOR2
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
					state = moving_to_2;
				}
				if (Rxmsg.can_id == ID_CC_TO_SC && Rxmsg.data[0] == 0x02) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
					state = moving_to_2;
				}

				if (Rxmsg.can_id == ID_F3_TO_SC && Rxmsg.data[0] == 0x01) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
					state = moving_to_3;
				}

				if (Rxmsg.can_id == ID_CC_TO_SC && Rxmsg.data[0] == 0x03) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
					state = moving_to_3;
				}
				break;

			case moving_to_1:
				printf("Moving to 1\n");
				if (Rxmsg.can_id == ID_EC_TO_ALL && Rxmsg.data[0] == AT_FLOOR1) {
					state = waiting_at_1;
				}

				break;
			case waiting_at_2:
				printf("Waiting at 2\n");
				if (Rxmsg.can_id == ID_F1_TO_SC && Rxmsg.data[0] == 0x01) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
					state = moving_to_1;
				}
				if (Rxmsg.can_id == ID_CC_TO_SC && Rxmsg.data[0] == 0x01) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
					state = moving_to_1;
				}

				if (Rxmsg.can_id == ID_F3_TO_SC && Rxmsg.data[0] == 0x01) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
					state = moving_to_3;
				}

				if (Rxmsg.can_id == ID_CC_TO_SC && Rxmsg.data[0] == 0x03) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
					state = moving_to_3;
				}
				break;
			case moving_to_2:
				printf("Moving to 2\n");
				if (Rxmsg.can_id == ID_EC_TO_ALL && Rxmsg.data[0] == AT_FLOOR2) {
					state = waiting_at_2;
				}

				break;
			case waiting_at_3:
				printf("Waiting at 3\n");
				if (Rxmsg.can_id == ID_F1_TO_SC && Rxmsg.data[0] == 0x01) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
					state = moving_to_1;
				}
				if (Rxmsg.can_id == ID_CC_TO_SC && Rxmsg.data[0] == 0x01) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
					state = moving_to_1;
				}

				if (Rxmsg.can_id == ID_F2_TO_SC && Rxmsg.data[0] == 0x01) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
					state = moving_to_2;
				}

				if (Rxmsg.can_id == ID_CC_TO_SC && Rxmsg.data[0] == 0x02) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
					state = moving_to_2;
				}

				break;
			case moving_to_3:
				printf("Moving to 3\n");
				if (Rxmsg.can_id == ID_EC_TO_ALL && Rxmsg.data[0] == AT_FLOOR3) {
					state = waiting_at_3;
				}

				break;
			case fault:
				printf("Error\n");
				// Handle fault condition (e.g. stop elevator, sound alarm, etc.)
				break;
			default:
				break;
			}
		}
		 else if (nbytes < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
			 perror("CAN read");
		}
	}
}

// Helper used by elevatoroperator (updated for SocketCAN)
int sendMsg(int id, int data, int sock) {
	struct can_frame frame = { 0 };
	frame.can_id = id;
	frame.can_dlc = 1;
	frame.data[0] = data;

	ssize_t n = write(sock, &frame, sizeof(frame));
	if (n != sizeof(frame)) {
		perror("sendMsg write");
		return -1;
	}
	printf("sendMsg [Tx] ID: 0x%04X  DATA: 0x%02X  --> Sent!\n", id, data);
	return 0;
}