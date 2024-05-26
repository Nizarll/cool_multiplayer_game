#include "../libs/nnetwork.h"
#include "../libs/nrender.h"

#define u32_first_half   0x0000ffff
#define u32_second_half  0xffff0000

static auto curr_key = 0;
static auto current = (Player) {
	.pos = (Vector2) {.x = 0, .y = 0},
};

static uint16_t deserialize_u16(int8_t* buff, size_t size) {
	if (size < 2) {
	  printf("error ! : could not deserialize u16\n");
	  exit(EXIT_FAILURE);
	}
	return (uint16_t) (*buff & (*(buff + 1) << 8));
}

static uint16_t deserialize_u32(int8_t* buff, size_t size) {
	if (size < 2) {
	  printf("error ! : could not deserialize u32\n");
	  exit(EXIT_FAILURE);
	}
	return (uint32_t)(deserialize_u16(buff, size)
			& deserialize_u16(buff + 2, size - 2) << 16);
}

static void serialize_u16(uint16_t var, int8_t* buff, size_t size) {
	if (size < 2) {
	  printf("error ! : could not serialize u16\n");
	  exit(EXIT_FAILURE);
	}
	*buff = var & 0xff;
	*(buff + 1) = ((var << 8) & 0xff00) >> 8;
}

static void serialize_u32(uint32_t var, int8_t* buff, size_t size) {
	if (size < 4) {
	  printf("error ! : could not serialize u16\n");
	  exit(EXIT_FAILURE);
	}
	serialize_u16((uint16_t)(var & u32_first_half), buff, size);
	serialize_u16((uint16_t)((var & u32_second_half) >> 16), buff + 2, size - 2);
}

void packet_serialize(Packet p, int8_t* arr, size_t size)
{
	*arr = p.kind;
	if (p.kind == DEMAND_CON) {
		return;
	}
	if (p.kind == INPUT_PRS) {
		*(arr + 1) == p.payload.input_key;
	}
}

Packet packet_deserialize(int8_t* buffer, size_t size)
{
	Packet p = (Packet) {
		.kind =*buffer,
	};
	if (p.kind == ALLOW_CON) {
		printf("allow con omfg\n");
		return p;
	}
	if (p.kind == MOVE) {
		int8_t* curr = buffer;
		uint16_t x = deserialize_u16(buffer = buffer + 1, size - 1);
		uint16_t y = deserialize_u16(buffer = buffer + 2, size - 1);
		return p;
	}
	if (p.kind == STATE_CHANGE) {
		return p;
	}

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

Packet receive_packet(Server* server, int8_t* buff, size_t size, struct sockaddr_in* addr) {
  int sock_size = sizeof(struct sockaddr_in);
	if(recvfrom(server->socket, buff, size, 0, addr, &sock_size) < 0) return (Packet){.kind = NONE};
  return (Packet)packet_deserialize(buff, size);
}

void send_packet(Server* server, Packet p, int8_t* buff, size_t buff_size) {
  int sock_size = sizeof(struct sockaddr_in);
  packet_serialize(p, buff, buff_size);
  if (sendto(server->socket, buff, buff_size, 0,
        &server->addr, sock_size) < 0)
    			printf("could not send packet to server\n");
}


void handle_input(Server *server, int8_t* buff, size_t size, struct sockaddr_in* addr)
{
  if(IsKeyDown(KEY_D) && curr_key != KEY_L) {
    send_packet(server,
      (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_R } }, buff, size);
    curr_key = KEY_L;
  }
  if(IsKeyDown(KEY_A) && curr_key != KEY_R) {
	  send_packet(server,
    (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_L } }, buff, size);
	curr_key= KEY_R;
  }
  if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && curr_key != KEY_MB1) {
	  send_packet(server,
    (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_MB1 } }, buff, size);
	  curr_key = KEY_MB1;
  }
  if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && curr_key != KEY_MB2) {
	  send_packet(server,
    (Packet) { .kind = INPUT_PRS, .payload = { .input_key = KEY_MB2 } }, buff, size);
	  curr_key = KEY_MB2;
  }
}

void demand_con(Server* server) {
	constexpr int buff_size = 1024;
	char buff[buff_size] = {0};
	auto p = (Packet) {.kind = DEMAND_CON};
	packet_serialize(p, buff, buff_size);
	while (p.kind != ALLOW_CON) {
		sendto(server->socket,
			 buff, buff_size, 0, &server->addr, sizeof(server->addr));
		int i = recvfrom(server->socket,
			  buff, buff_size, 0, NULL, NULL);
		printf("\n\n");
		p = packet_deserialize(buff, buff_size);
		printf("ALLOWCON: %d packet: %d", ALLOW_CON, p.kind);
	}
#if defined(OS_PLATFORM_UNIX)
	fcntl(server->socket, F_SETFL, O_NONBLOCK); 
#else
	unsigned long non_blocking = 1;
	ioctlsocket(server->socket, FIONBIO, &non_blocking);
#endif
}
