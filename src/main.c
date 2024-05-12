#include "../libs/main.h"

#define ARR_LEN(t) (sizeof(t) / sizeof(t[0]))

constexpr auto height = 400;
constexpr auto width = 400;
constexpr auto size = 30;

const auto screenHeight = height;
const auto screenWidth = width;

static auto runSheet = &(SpriteAnimation) {
	.path = "resources\\run.png",
		.size = (Vector2){.x = 16.0f, .y = 16.0f},
		.offset = (Vector2){.x = 64.0f, .y = 64.0f},
		.position = (Vector2){},
		.counter = 0, .length = 8,
};


int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(width, height, "hello world");
	SetTargetFPS(144);
	auto pool = mempool_create(1024);
	auto animation = animation_create(eINOUT_BACK,
			true,
			NULL,
			0,
			pool);
	animation->duration = .0f;
	animation->paused = false;
	spriteanim_load(runSheet);
	while(!WindowShouldClose()) {
		Keyframe walk_anim[] = {
			(Keyframe) {
					.from = (Vector2) {.x = 200.0f, .y = 200.0f},
					.to = (Vector2) {.x = 200.0f, .y = 250.0f},
					.duration = 20
			},
				(Keyframe) {
					.from = (Vector2) {.x = 200.0f, .y = 250.0f},
					.to = (Vector2) {.x = 200.0f, .y = 200.0f},
					.duration = 20
				}
		};
		animation->looped = true;
		animation->keyframes = walk_anim;
		animation->kf_count = ARR_LEN(walk_anim);
		animation_update(animation);
		BeginDrawing();
		ClearBackground((Color){0x88, 0x88, 0x88, 0xff});
		//spriteanim_update(runSheet);
		//printf("%.2f\n", animation->current.y);
		{
			if (runSheet->counter < runSheet->length * 10)
				runSheet->counter += 1;
			else runSheet->counter = 0;
			float frame_x;
			float frame_y;
			frame_y =	runSheet->offset.y / 2;
			if (runSheet->counter == 0) {
				frame_x = runSheet->offset.x / 2;
			} else {
				frame_x = (runSheet->counter / 10) 
					* (runSheet->offset.x + 16.0f) + 32.0f;
			}
			DrawTexturePro(runSheet->sheet,
					(Rectangle){(float)frame_x,
					(float)frame_y, 16.0f, 16.0f },
					(Rectangle) { screenWidth / 2.0f, screenHeight / 2.0f, 200.0f, animation->current.y },
					(Vector2){animation->current.x / 2, animation->current.y / 2},
					0.0f,
					WHITE);

		}

		//DrawCircle(animation->current.x, GetScreenHeight() / 2, animation->current.x/ 10, BLACK);
		EndDrawing();
	}
	CloseWindow();
	mempool_destroy(pool);
	return 0;
}
