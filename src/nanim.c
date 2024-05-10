#include "../libs/nanim.h"
#include <math.h>
#include <raylib.h>

/*******************|| EASING FUNCTIONS ||*******************/

static float linear(float t) {return t;}
static float ease_inout_back(float x) {
	constexpr auto c1 = 1.70158f;
	constexpr auto c2 = c1 * 1.525f;
	return x < .5f
				? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2.0f
				: (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2.0f;
}
static float ease_inout_cubic(float x) {
	return x < 0.5f ? 4.0f * x * x * x : 1 - powf(-2 * x + 2, 3) / 2.0f;
}
static float ease_inout_circ(float x) {
return x < 0.5f
  ? (1.0f - sqrtf(1 - powf(2 * x, 2))) / 2.0f
  : (sqrtf(1.0f - powf(-2 * x + 2, 2.0f)) + 1.0f) / 2.0f;
}
static float ease_inout_exp(float x) {
	return x == .0f ? .0f : x == 1.0f ? 1.0f
  : x < 0.5f ? powf(2, 20 * x - 10) / 2.0f
  : (2 - powf(2, -20 * x + 10)) / 2.0f;
}
static float lerp(float from, float to, float t) {
	return (from + (to - from) * t);
}

/*******************|| EASING FUNCTIONS ||*******************/

static float (*easing_functions[])(float) = {
	[eLINEAR] = &linear,
	[eINOUT_BACK] = &ease_inout_back,
	[eINOUT_CUBIC] = &ease_inout_cubic,
};

Vector2 vector2_lerp(Vector2 from, Vector2 to, float t, EasingStyle style) {
	return (Vector2) {
		.x = lerp(from.x, to.x,(easing_functions[style])(t)), 
		.y = lerp(from.y, to.y, (easing_functions[style])(t)), 
	};
}

void update_animation(Animation* anim) {
	if (anim->paused) return;
	if (anim->index >= anim->kf_count && anim->looped) {
		anim->index = 0;
		anim->duration = .0f;
	}
	auto keyframe = &anim->keyframes[anim->index];
	auto dt = GetFrameTime();
	anim->duration += GetFrameTime();
	anim->current = vector2_lerp(keyframe->from,
															keyframe->to,
															anim->duration/keyframe->duration,
															eINOUT_CUBIC);
	while (anim->index < anim->kf_count && anim->duration >= anim->keyframes[anim->index].duration) {
		anim->index += 1;
		anim->duration -= keyframe->duration;
		keyframe = &anim->keyframes[anim->index];
	}
}

void play_animation(Animation* anim) {
	anim->paused = false;
}
void pause_animation(Animation* anim) {
	anim->paused = true;
}
