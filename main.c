#include "main.h"

#define ARR_LEN(t) (sizeof(t) / sizeof(t[0]))

constexpr auto height = 400;
constexpr auto width = 400;
constexpr auto size = 30;

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	auto pool = mempool_create(1024);
	auto animation = animation_create(eINOUT_BACK, true, nullptr, 0, pool);
	InitWindow(width, height, "hello world");
	SetTargetFPS(120);
	while(!WindowShouldClose()) {
		float ratio = GetScreenWidth() / width;
		Keyframe kfs[] = {
			(Keyframe)
			{
				.from = (Vector2){.x = 2 * size },
				.to = (Vector2){.x = GetScreenWidth() - 2 * size},
				.duration = ratio
			},
			(Keyframe)
			{
				.from = (Vector2){.x = GetScreenWidth() - 2 * size},
				.to = (Vector2){.x = 2 * size },
				.duration = ratio
			},
			(Keyframe)
			{
				.from = (Vector2){.x = 2 * size },
				.to = (Vector2){.x = GetScreenWidth()/ 3 },
				.duration = ratio
			},
			(Keyframe)
			{
				.from = (Vector2){.x = GetScreenWidth()/ 3 },
				.to = (Vector2){.x = 2 * size },
				.duration = ratio
			},
		};
		animation->keyframes = kfs;
		animation->kf_count = ARR_LEN(kfs);
		animation_update(animation);
		BeginDrawing();
		ClearBackground((Color){0x88, 0x88, 0x88, 0xff});
		DrawCircle(animation->current.x, GetScreenHeight() / 2, animation->current.x/ 10, BLACK);
		EndDrawing();
	}
	CloseWindow();
	mempool_destroy(pool);
	return 0;
}
