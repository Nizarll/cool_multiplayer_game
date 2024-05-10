#ifndef NANIM_H
#define NANIM_H

#ifndef MAX_VTABLE_IMPL
constexpr auto MAX_VTABLE_IMPL = 10;
#endif

#include <stdio.h>
#include <raylib.h>
#include <stdint.h>

/*
 * Virtual tables allow us to implement
 * so that each rlobjecttype instance could have
 * its own implementation of the render and destroy
 * functions
 */

typedef enum : int8_t {
	eLINEAR = 0,
	eINOUT_CIRC,
	eINOUT_CUBIC,
	eINOUT_EXP,
	eINOUT_BACK,
	EASE_ENUM_LEN
} EasingStyle;

/***********************************/

/* ANIMATION LIBRARY IMPLEMENTATION IN C */

typedef struct {
	Vector2 from;
	Vector2 to;
	size_t duration;
} Keyframe;

typedef struct {
	Keyframe* keyframes;
	Vector2 current;
	EasingStyle style;
	float duration;
	size_t kf_count;
	size_t index;
	bool looped;
	bool paused;
} Animation;

typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	Vector2 size;
	Color color;
	size_t roundness;
}NButton;

typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	Vector2 size;
	//Image imp
}NImgButton;

typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	Vector2 size;
	Color color;
}NRect;

typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	size_t radius;
	Color color;
}NCircle;

/***********************************/

void update_animation(Animation* anim);
void play_animation(Animation* anim);
void pause_animation(Animation* anim);

#endif
