#include "../libs/nnetwork.h"

#define u32_first_half  0xffff0000
#define u32_second_half 0xffff0000

int curr_key = 9999;

static void serialize_u16(uint16_t var, int8_t* buff, size_t size) {
	if (size < 2) {
	  printf("error ! : could not serialize u16\n");
	  exit(EXIT_FAILURE);
	}
	*buff = var & 0xff;
	*(buff + 1) = (var << 8) & 0xff00;
}

static void serialize_u32(uint32_t var, int8_t* buff, size_t size) {
	if (size < 4) {
	  printf("error ! : could not serialize u16\n");
	  exit(EXIT_FAILURE);
	}
	serialize_u16((uint16_t)(var & u32_first_half) >> 16, buff, size);
	serialize_u16((uint16_t)(var & u32_second_half), buff + 2, size - 2);
}

void packet_serialize(Packet* p, int8_t* arr, size_t size)
{
	*arr = p->kind;
	switch(p->kind) {
		case INPUT_PRS:
		  *(arr + 1) = p->payload.input_key;
		  break;
		default:
		  printf("unimplemented");
		  break;
	}
}

Packet packet_deserialize(int8_t* buffer, size_t size)
{
	Packet p = (Packet) {
		.kind =*buffer,
	};
	return p;
}

void send_packet(Server* server, Packet p, int8_t* buff, size_t buff_size)
{
  printf("sending packet : %d\n", p.kind);
  packet_serialize(&p, buff, buff_size);
  sendto(server->socket,
    buff,
    buff_size,
    0,
    (sockaddr *) &server->addr,
    sizeof(server->addr));
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

void handle_input(Server *server, int8_t* buff, size_t size)
{
  if(IsKeyDown(KEY_D)) {
    send_packet(server,
      (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_R } }, buff, size);
    curr_key = KEY_L;
  }
  if(IsKeyDown(KEY_A)) {
	  send_packet(server,
    (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_L } }, buff, size);
	curr_key= KEY_R;
  }
  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
	  send_packet(server,
    (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_MB1 } }, buff, size);
	  curr_key = KEY_R;
  }
  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
	  send_packet(server,
    (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_MB2 } }, buff, size);
	  curr_key = KEY_L;
  }
}

void demand_con(Server* server) {
	constexpr int buff_size = 1024;
	char buff[buff_size] = {0};
	auto p = (Packet) {.kind = DEMAND_CON};
	packet_serialize(&p, buff, buff_size);
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
