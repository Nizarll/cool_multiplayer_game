#include "main.h"
#include <stdlib.h>

#define ARR_LEN(t) (sizeof(t) / sizeof(t[0]))

constexpr auto height = 400;
constexpr auto width = 400;
constexpr auto size = 30;

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(width, height, "hello world");
	SetTargetFPS(60);
	Animation *animation = malloc(sizeof(animation));
	animation->kf_count = 2;
	animation->duration = 0;
	animation->paused = false;
	play_animation(animation);
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
		};
		animation->keyframes = kfs;
		update_animation(animation);
		BeginDrawing();
		ClearBackground((Color){0x88, 0x88, 0x88, 0xff});
		DrawCircle(animation->current.x, GetScreenHeight() / 2, 30, BLACK);
		EndDrawing();
	}
	free(animation);
	CloseWindow();
	return 0;
}
