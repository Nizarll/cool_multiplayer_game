#include "../libs/main.h"

#define ARR_LEN(t) (sizeof(t) / sizeof(t[0]))

constexpr auto height = 400;
constexpr auto width = 400;

void window_init() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(width, height, "hello world");
	SetTargetFPS(144);
}

int main() {
	auto pool = mempool_create(1024);
	auto server = (Server) {
		.port = 2356,
		.ip = "31.207.36.91",
	};
	server_init(&server);
	window_init();
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground((Color){0x88, 0x88, 0x88, 0xff});
		EndDrawing();
	}
	mempool_destroy(pool);
	CloseWindow();
	return 0;
}
