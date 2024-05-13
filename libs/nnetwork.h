#ifndef NNETWORK_H
#define NNETWORK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define UNIMPLEMENTED

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h> defined(__APPLE__)
#endif

typedef enum : char {
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
	VFX, // packet sent to tell that there's vfx to be summoned
	PACKET_LEN
} PacketKind;

UNIMPLEMENTED typedef struct {
} Server;

typedef struct {
	void* data;
	size_t occupied;
	size_t length;
} PacketBuffer;

typedef struct {
	const char const * payload; // read only
	uint32_t size;
	uint32_t type;
} Packet; // no need for pointer small enough to be copiable

void encode(Packet* p);
void decode(Packet* p);
const char const * packet_serialize(Packet* p);
Packet* packet_serialize(Packet* p);

#endif
