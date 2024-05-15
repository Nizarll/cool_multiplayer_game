#ifndef NNETWORK_H
#define NNETWORK_H

#include "utils.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define UNIMPLEMENTED

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#define OS_PLATFORM_WINDOWS
#include <windows.h>
#include <winsock2.h>
#else
#define OS_PLATFORM_UNIX 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
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
	DEMAND_CON = 0,
	ALLOW_CON,
	JOIN,
	LEAVE,
	STATE_CHANGED,
	VFX,
	PACKET_LEN
} PacketKind;

UNIMPLEMENTED typedef struct {
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
		uin32_t input_key;
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
static void packet_serialize(Packet* p, int8_t* arr);
static Packet* packet_deserialize(Packet* p);

Packet* receive_packet(Server* server);
void server_init(Server* server);

#endif
