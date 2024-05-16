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

static Packet packet_deserialize(uint8_t* buff, size_t size)
{
	auto p = (Packet) {
		.kind = *buff
	};
	return p;
}

void server_init(Server* server) {
#if defined(OS_PLATFORM_UNIX)
	if((server->socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error !: failed to create socket\n");
		exit(EXIT_FAILURE);
	}
#else
	if(WSAStartup(MAKEWORD(2,2), &server->data) != 0) {
		printf("error !: wsastartup failed errorcode :%d\n",
				WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	server->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (server->socket == INVALID_SOCKET) {
		printf("error !: failed to create socket errorcode :%d\n",
				WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}
#endif
	server->addr.sin_family = AF_INET;
	server->addr.sin_port = htons(server->port);
	server->addr.sin_addr.s_addr = inet_addr(server->ip);
	demand_con(server);
}

void demand_con(Server* server) {
	char buff[32] = {0};
	Packet p = {
		.kind = 2,
		.size = ARR_LEN(buff),
	};
	
	packet_serialize(&p, buff, ARR_LEN(buff));
	while (p.kind != ALLOW_CON ) {
		char* bytes = sendto(server->socket,
						buff,
						ARR_LEN(buff),
						0,
						(sockaddr *) &server->addr,
						sizeof(server->addr)
					);
		printf("sent: %c\n", *buff + '0');
		recv(server->socket, buff, ARR_LEN(buff), 0);
		p = packet_deserialize(buff, ARR_LEN(buff));
		printf("\n packet type is %d", p.kind);
	}
}
