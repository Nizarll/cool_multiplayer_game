#include "../libs/nanim.h"

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
	[eINOUT_EXP] = &ease_inout_exp,
};

Vector2 vector2_lerp(Vector2 from, Vector2 to, float t, EasingStyle style) {
	return (Vector2) {
		.x = lerp(from.x, to.x,(easing_functions[style])(t)), 
			.y = lerp(from.y, to.y, (easing_functions[style])(t)), 
	};
}

Animation* animation_create(EasingStyle style,
		bool looped,
		Keyframe* keyframes,
		size_t kf_count,
		Mempool* pool) {
	Animation* anim;
	if (style >= EASE_ENUM_LEN) {
		printf("error! : easing function is not a valid easing function !");
		exit(EXIT_FAILURE);
	}
	if (pool) {
		anim = mempool_alloc(pool, sizeof(Animation));
	} else {
		anim = malloc(sizeof(Animation));
	}
	if (!anim) {
		printf("error! : could not create animation object" "\n");
		exit(EXIT_FAILURE);
	}
	
	anim->keyframes = keyframes;
	anim->kf_count = kf_count;
	anim->style = style;
	anim->index = 0;
	anim->duration = .0f;
	anim->_internal = true;
	anim->current = (Vector2){};
	anim->looped = looped;
	anim->paused = false;

	return anim;
}

void animation_update(Animation* anim) {
	if(!anim->_internal) {
		printf("error! : animation must be created through constructor" "\n");
		exit(EXIT_FAILURE);
	}
	if (anim->paused) return;
	if (anim->index >= anim->kf_count && anim->looped) {
		anim->index = 0;
		anim->duration = .0f;
	}
	auto keyframe = &anim->keyframes[anim->index];
	auto dt = GetFrameTime();
	anim->duration += dt;
	anim->current = vector2_lerp(keyframe->from,
			keyframe->to,
			anim->duration /
				(keyframe->duration / 100.0f),
			eLINEAR);
	while (anim->index < anim->kf_count && anim->duration * 100.0f >=anim->keyframes[anim->index].duration) {
		anim->index += 1;
		anim->duration -= keyframe->duration / 100.0f;
		keyframe = &anim->keyframes[anim->index];
	}
}

void animation_play(Animation* anim) {
	anim->paused = false;
}
void animation_pause(Animation* anim) {
	anim->paused = true;
}
