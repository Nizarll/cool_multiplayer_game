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
#define recv(...) recvfrom(__VA_ARGS__)
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
	DISALLOW = 0,
	ALLOW_CON,
	DEMAND_CON,
	DEMAND_DISCON,
	JOIN,
	LEAVE,
	STATE_CHANGED,
	VFX,
	PACKET_LEN
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
		StatePacketData data;
		Vector2 position;
		Vector2 size;
		uint32_t input_key;
	} payload;
	uint32_t size;
	uint32_t kind;
} Packet;

typedef struct {
	void* data;
	size_t length;
	size_t kind;
} State;

static void encode(Packet* p);
static void decode(Packet* p);
static void packet_serialize(Packet* packet, int8_t* arr, size_t size);
static Packet packet_deserialize(uint8_t* buffer, size_t size);

//Packet* receive_packet(Server* server);
void server_init(Server* server);
#endif
