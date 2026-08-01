#include "../include/pcanFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string>
#include <stdio.h>

// Use VLC to play audio files instead of the ALSA-based playAudio implementation
static void playAudioVLC(const char* filename,int repeat) {
	if (!filename) return;
	char cmd[1024];
	// Use dummy interface; disable loop/repeat and exit when finished so the file does not replay
	// --no-media-library avoids indexing, --quiet reduces output
	if(repeat==0)
		snprintf(cmd, sizeof(cmd), "vlc --intf dummy --no-loop --no-repeat --play-and-exit --no-media-library --quiet \"%s\" >/dev/null 2>&1 &", filename);
	else
		snprintf(cmd, sizeof(cmd), "vlc --intf dummy --loop --repeat --no-media-library --quiet \"%s\" >/dev/null 2>&1 &", filename);
	system(cmd);
}

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

	//system("@cls||clear");
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
	arrived_at_1_moving_down_door_open,
	arrived_at_1_moving_down_door_closed,
	moving_down_to_1,
	arrived_at_2_moving_up_door_open,
	arrived_at_2_moving_up_door_closed,
	arrived_at_2_moving_down_door_open,
	arrived_at_2_moving_down_door_closed,
	moving_down_to_2,
	moving_up_to_2,
	arrived_at_3_moving_up_door_open,
	arrived_at_3_moving_up_door_closed,
	moving_up_to_3,
	fault
};

void elevatoroperator() {
	if (can_open() < 0) {
		printf("Failed to open CAN socket!\n");
		return;
	}

	struct can_frame Rxmsg;
	enum State state = initial;
	int word = 0;
	int sabbath_mode = 0; // 0 = normal, 1 = sabbath mode
	int maintenance_lock_out = 0; // 0 = normal, 1 = maintenance lock-out
	int previousState=0;
	playAudioVLC("../../../audio/elevator.mp3", 1);

	printf("\nElevator Operator State Machine Started (SocketCAN)\n");
	printf("Press Ctrl+C to exit\n\n");

	while (1) {
		ssize_t nbytes = read(can_socket, &Rxmsg, sizeof(Rxmsg));

		if (nbytes == sizeof(Rxmsg)) {
			printf("Rx ID: 0x%X  Data: 0x%02X\n", Rxmsg.can_id, Rxmsg.data[0]);

			//receive message and add to word
			if (Rxmsg.can_id == ID_F1_TO_SC) {
				word |= Rxmsg.data[0] << 16;
			}
			if (Rxmsg.can_id == ID_F2_TO_SC) {
				word |= Rxmsg.data[0] << 12;
			}
			if (Rxmsg.can_id == ID_F3_TO_SC) {
				word |= Rxmsg.data[0] << 8;
			}
#define F1U (word & (UP << 16))
//#define F1D	(word & (DOWN << 16))
//#define F1C (word & (REQUEST << 16))
#define F2U (word & (UP << 12))
#define F2D	(word & (DOWN << 12))
//#define F2C (word & (REQUEST << 12))
//#define F3U (word & (UP << 8))
#define F3D (word & (DOWN << 8))
//#define F3C (word & (REQUEST << 8))

#define F1 (word & (0x01 << 4))
#define F2 (word & (0x02 << 4))
#define F3 (word & (0x04 << 4))

#define O (word & (0x01<<20))
#define C (word & (0x02<<20))

			if (Rxmsg.can_id == ID_CC_TO_SC) {
				if (Rxmsg.data[0] == 0x01) word |= 0x01 << 4; // F1
				else if (Rxmsg.data[0] == 0x02) word |= 0x02 << 4; // F2
				else if (Rxmsg.data[0] == 0x03) word |= 0x04 << 4; // F3
			}

			if (Rxmsg.can_id == ID_CC_TO_SC_DOOR) {
				if (Rxmsg.data[0] == 0x00) word |= 0x01 << 20;//O
				else if (Rxmsg.data[0] == 0x01) word |= 0x02 << 20;//C
			}

			if (Rxmsg.can_id == ID_EC_TO_ALL) {
				word = (word & 0xfffffff0) | Rxmsg.data[0];
			}
			/*if (Rxmsg.can_id == ID_MODE) {
				if (Rxmsg.data[0] == 0x01){
					sabbath_mode = 1; 
					maintenance_lock_out = 0;
				}
				else if (Rxmsg.data[0] == 0x02){
					sabbath_mode = 0;
					maintenance_lock_out = 1;
					printf("The elevator is in maintenance lock-out mode. No operations will be performed.\n");
					return;
				}
				else if (Rxmsg.data[0] == 0x00) {
					sabbath_mode = 0;
					maintenance_lock_out = 0;
				}
			}*/

			switch (state) {
			case initial:
				printf("Initial\n");//Initialize elevator
				if ((word & 0x0F) == AT_FLOOR1) state = arrived_at_1_moving_down_door_open;
				else if ((word & 0x0F) == AT_FLOOR2) state = arrived_at_2_moving_down_door_open;
				else if ((word & 0x0F) == AT_FLOOR3) state = arrived_at_3_moving_up_door_open;
				else if (maintenance_lock_out == 1) break;
				break;

			case arrived_at_1_moving_down_door_open:
				printf("word: %08x state: Arrived at 1, moving down door open\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				if (previousState != 10) {
					playAudioVLC("../../../audio/arrived_at_1.wav", 0);
					printf("previous state not current state");
				}
				previousState = 10;
				if (C || F2U || F2D || F2 || F3D || F3 || (sabbath_mode==1)) {//close door if close door is pressed or elevator is called to any other floor
					state = arrived_at_1_moving_down_door_closed;
					word &= ~C;
				}
				break;

			case arrived_at_1_moving_down_door_closed:
				printf("word: %08x state: Arrived at 1, moving down door closed\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				previousState = 11;
				if (F1U || O) {
					state = arrived_at_1_moving_down_door_open;
					word &= ~(F1U | O);
				}
				// Waiting at floor 1
				// If command received, state = moving_to_2 or 3
				if (F2U || F2D || F2 || (sabbath_mode == 1)) { // FC2 is calling
					// now send message to EC to MOVE TO FLOOR 2: CAN ID 0X100, MESSAGE BYTE GO_TO_FLOOR2
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
					state = moving_up_to_2;
					word &= ~(F1U | F1);
				}

				else if (F3D || F3) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
					state = moving_up_to_3;
					word &= ~(F1U | F1);
				}
				break;

			case arrived_at_2_moving_down_door_open:
				printf("word: %08x state: Arrived at 2, moving down door open\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				if (previousState != 20) {
					playAudioVLC("../../../audio/arrived_at_2.wav", 0);
					printf("previous state not current state");
				}
				previousState = 20;
				if (C || F1U || F1 || F3 || F3D || (sabbath_mode == 1)) {
					state = arrived_at_2_moving_down_door_closed;
					word &= ~C;
				}
				break;

			case arrived_at_2_moving_down_door_closed:
				printf("word: %08x state: Arrived at 2, moving down door closed\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				previousState = 21;
				if (F2U || F2D || O) {
					state = arrived_at_2_moving_down_door_open;
					word &= ~(F2U | F2D | O);
				}
				if (F1U || F1 || (sabbath_mode == 1)) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
					state = moving_down_to_1;
					word &= ~(F2D | F2);
				}
				else if ((F3 || F3D) && (F1U == 0) && (F1 == 0)) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
					state = moving_up_to_3;
					word &= ~(F2D | F2);
				}
				break;

			case arrived_at_2_moving_up_door_open:
				printf("word: %08x state: Arrived at 2, moving up door open\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				if (previousState != 30) {
					playAudioVLC("../../../audio/arrived_at_2.wav", 0);
					printf("previous state not current state");
				}
				previousState = 30;
				if (C || F1U || F1 || F3 || F3D || (sabbath_mode == 1)) {
					state = arrived_at_2_moving_up_door_closed;
					word &= ~C;
				}
				break;

			case arrived_at_2_moving_up_door_closed:
				printf("word: %08x state: Arrived at 2, moving up door closed\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				previousState = 31;
				if (F2U || F2D || O) {
					state = arrived_at_2_moving_up_door_open;
					word &= ~(F2U | F2D | O);
				}
				if (F3D || F3 || (sabbath_mode == 1)) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
					state = moving_up_to_3;
					word &= ~(F2U | F2);
				}
				else if ((F1 || F1U) && (F3D == 0) && (F3 == 0)) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
					state = moving_down_to_1;
					word &= ~(F2U | F2);
				}
				break;

			case arrived_at_3_moving_up_door_open:
				printf("word: %08x state: Arrived at 3, moving up door open\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				if (previousState != 40) {
					playAudioVLC("../../../audio/arrived_at_3.wav", 0);
					printf("previous state not current state");
				}
				previousState = 40;
				if (C || F2U || F2D || F2 || F1U || F1 || (sabbath_mode == 1)) {
					state = arrived_at_3_moving_up_door_closed;
					word &= ~C;
				}
				break;

			case arrived_at_3_moving_up_door_closed:
				printf("word: %08x state: Arrived at 3, moving up door closed\n", (unsigned int)word);
				if (maintenance_lock_out == 1) break;
				previousState = 41;
				if (F3D || O) {
					state = arrived_at_3_moving_up_door_open;
					word &= ~(F3D | O);
				}
				if (F2U || F2D || F2 || (sabbath_mode == 1)) {
					// now send message to EC to MOVE TO FLOOR 2: CAN ID 0X100, MESSAGE BYTE GO_TO_FLOOR2
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
					state = moving_down_to_2;
					word &= ~(F3D | F3);
				}

				else if (F1U || F1) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
					state = moving_down_to_1;
					word &= ~(F3D | F3);
				}
				break;

			case moving_down_to_1:
				printf("word: %08x state: Moving down to 1\n", (unsigned int)word);
				if ((word & 0x0F) == AT_FLOOR1) {
					state = arrived_at_1_moving_down_door_open;
				}
				else if (((word & 0x0F) == AT_FLOOR2) || ((word & 0x0F) == AT_FLOOR3)) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);
				}
				else if ((word & 0x04) == 0) { //disabled somehow??
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR1, can_socket);  // wake up, slacker, hut hut hut
				}
				break;

			case moving_down_to_2:
				printf("word: %08x state: Moving down to 2\n", (unsigned int)word);
				if ((word & 0x0F) == AT_FLOOR2) {
					state = arrived_at_2_moving_down_door_open;
				}
				else if (((word & 0x0F) == AT_FLOOR1) || ((word & 0x0F) == AT_FLOOR3)) {//enabled but at a different floor
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
				}
				else if ((word & 0x04) == 0) { //disabled somehow??
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);  // wake up, slacker, hut hut hut
				}
				break;

			case moving_up_to_2:
				printf("word: %08x state: Moving up to 2\n", (unsigned int)word);
				if ((word & 0x0F) == AT_FLOOR2) {
					state = arrived_at_2_moving_up_door_open;
				}
				else if (((word & 0x0F) == AT_FLOOR1) || ((word & 0x0F) == AT_FLOOR3)) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);
				}
				else if ((word & 0x04) == 0) { //disabled somehow??
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR2, can_socket);  // wake up, slacker, hut hut hut
				}
				break;

			case moving_up_to_3:
				printf("word: %08x state: Moving up to 3\n", (unsigned int)word);
				if ((word & 0x0F) == AT_FLOOR3) {
					state = arrived_at_3_moving_up_door_open;
				}
				else if (((word & 0x0F) == AT_FLOOR1) || ((word & 0x0F) == AT_FLOOR2)) {
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);
				}
				else if ((word & 0x04) == 0) { //disabled somehow??
					sendMsg(ID_SC_TO_EC, GO_TO_FLOOR3, can_socket);  // wake up, slacker, hut hut hut
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
	usleep(250000);//make sure message gets sent some time after the previous message was sent because the arduino might be bad at receiving many messages quickly
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