#ifndef NNETWORK_H
#define NNETWORK_H

#define UNIMPLEMENTED
#include "utils.h"
#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>
#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#define OS_PLATFORM_WINDOWS
#define sockaddr SOCKADDR
#define Rectangle boohoo_windows
#define CloseWindow win32_t_sucks 
#define ShowCursor win32_t_sucks2 
#include <winsock2.h>
#undef CloseWindow
#undef Rectangle
#undef ShowCursor
#else
#define OS_PLATFORM_UNIX
#define sockaddr struct sockaddr
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

#ifndef ARR_LEN
#define ARR_LEN(t) (sizeof(t) / sizeof(*t))
#endif

#define KEY_R      0x01
#define KEY_L      0x02
#define KEY_ATTCK1 0x03
#define KEY_ATTCK2 0x04
#define KEY_SP1    0x05
#define KEY_SP2    0x06
#define KEY_MB1	   0x07
#define KEY_MB2	   0x08
#define KEY_JMP    0x09

typedef enum {
	WALK = 0,
	IDLE,
	RUN,
	JUMP,
	ATTACK,
	BLOCK,
	PARRY,
	STATE_ENUM_LEN
} StateKind;

typedef enum {
	DISALLOW = 0x00,
	ALLOW_CON = 0x01,
	DEMAND_CON = 0x02,
	DEMAND_DISCON = 0x03,
	INPUT_PRS = 0x04,
	JOIN = 0x05,
	LEAVE = 0x06,
	MOVE = 0x07,
	STATE_CHANGE = 0x08,
	VFX = 0x09,
	PACKET_LEN = 0x0A,
} PacketKind;

UNIMPLEMENTED typedef struct {
	const char* ip;
#if defined(OS_PLATFORM_UNIX)
	struct sockaddr_in addr;
	size_t port;
	size_t socket;
#else
	WSADATA data;
	SOCKET socket;
	SOCKADDR_IN addr;
	size_t port;
#endif
} Server;

typedef struct {
	void* data;
	size_t occupied;
	size_t length;
} PacketBuffer;

typedef struct {
	uint32_t player_id;
	uint32_t state_id;
	Vector2 position;
} StatePacketData;

typedef struct {
	union {
		uint8_t input_key;
		Vector2 position;
		Vector2 size;
		StatePacketData data;
	} payload;
	uint32_t size;
	uint32_t kind;
} Packet;

typedef struct {
	void* data;
	size_t length;
	size_t kind;
} State;

typedef struct {
	Vector2 pos;
	size_t id;
} Player;

static Packet packet_deserialize(int8_t* buffer, size_t size);
static void packet_serialize(Packet* packet, int8_t* arr, size_t size);
static void send_packet(Server* server, Packet p, int8_t* buff, size_t size);
static void demand_con(Server* server);
void handle_input(Server *server, int8_t* buff, size_t size);
void server_init(Server* server);
#endif
