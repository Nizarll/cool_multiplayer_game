#include "../libs/main.h"

#define ARR_LEN(t) (sizeof(t) / sizeof(t[0]))

constexpr auto height = 400;
constexpr auto width = 400;
constexpr auto DEFAULT_PACKET_SIZE = 1024;

void window_init() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(width, height, "hello world");
	SetTargetFPS(144);
}

void clear_packetbuff(int8_t* buff){
	memset(buff, 0, sizeof(buff));
	*buff = NONE;
}

int main() {
	char buffer[DEFAULT_PACKET_SIZE] = {0};

	auto pool = mempool_create(1024);
	auto time_val = (struct timeval) {0};
	auto recv_addr = (struct sockaddr_in){0};
	auto server = (Server) {.port = SERVER_PORT,.ip = SERVER_IP};
	server_init(&server);
	window_init();
	while(!WindowShouldClose()) {
		FD_ZERO(&server.set);
		FD_SET(server.socket, &server.set);
		clear_packetbuff(buffer);
		handle_input(&server, buffer, DEFAULT_PACKET_SIZE, &recv_addr);
		Packet p = receive_packet(&server, buffer, DEFAULT_PACKET_SIZE, &recv_addr);
		if (p.kind != NONE) printf("received packet with id : %d\n", p.kind);
		BeginDrawing();
		ClearBackground((Color){0x88, 0x88, 0x88, 0xff});
		EndDrawing();
	}
	mempool_destroy(pool);
	CloseWindow();
	return EXIT_SUCCESS;
}
