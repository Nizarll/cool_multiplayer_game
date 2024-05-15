#include "../libs/nnetwork.h"

static void serialize_u16(uint16_t var) {

}

static void serialize_u32(uint16_t var) {

}
static void packet_serialize(Packet* p, int8_t* buff, size_t size)
{
	switch(p->kind) {
		case DEMAND_CON:
			*buff = DEMAND_CON;
			break;
		default:
			printf("unimplemented");
			break;
	}
}

static Packet packet_deserialize(uint8_t* p)
{
	switch(p->kind) {
		case STATE_CHANGE:
			//
			break;
	}
}

void server_init(Server* server) {
#if defined(OS_PLATFORM_UNIX)
	if((server->socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error !: failed to create socket\n");
		exit(EXIT_FAILURE);
	}
	server->addr.sin_family = AF_INET;
	server->addr.sin_port = htons(server->port);
	server->addr.sin_addr.s_addr = server->ip;
#else
	if(WSAStartup(MAKEWORD(2,2), server->data) != 0) {
		printf("error !: wsastartup failed errorcode :%ld\n",
				WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	server->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (server->socket == INVALID_SOCKET) {
		printf("error !: failed to create socket errorcode :%ld\n",
				WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}
#endif
}
