#include "../libs/nnetwork.h"

static void handle_input()
{
	
}

static void packet_serialize(Packet* p, int8_t* buff, size_t size)
{
	*buff = p->kind;
	switch(p->kind) {
		case INPUT:
			break;
		default:
			printf("unimplemented");
			break;
	}
}

static Packet packet_deserialize(uint8_t* buff, size_t size)
{
	auto p = (Packet) {
		.kind =*buff,
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
	constexpr int buff_size = 1024;
	char buff[buff_size] = {0};
	auto p = (Packet) {.kind = DEMAND_CON};
	serialize_packet(&p, buff, buff_size);
	while (buff[0] != ALLOW_CON) {
		sendto(server->socket,
						buff,
						buff_size,
						0,
						(sockaddr *) &server->addr,
						sizeof(server->addr)
					);
		int i = recvfrom(server->socket,
				buff,
				buff_size,
				0,
				(struct sockaddr *) NULL,
				NULL);
		printf("\n\n");
		p = packet_deserialize(buff, buff_size);
		printf("ALLOWCON: %d packet: %d", ALLOW_CON, p.kind);
	}
}
