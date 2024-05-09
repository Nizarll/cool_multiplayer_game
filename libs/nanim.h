#ifndef NANIM_H
#define NANIM_H

#ifndef MAX_VTABLE_IMPL
#define MAX_VTABLE_IMPL 10
#endif

#include <stdio.h>
#include <raylib.h>

#define NANIM_TYPE
#define NANIM_UIOBJ

/*
 * Virtual tables allow us to implement
 * so that each rlobjecttype instance could have
 * its own implementation of the render and destroy
 * functions
 */

/***********************************/

NANIM_TYPE typedef struct {
	Keyframe *keyframes;
	EasingStyle style;
	size_t i;
} Animation;

NANIM_UIOBJ typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	Vector2 size;
	Color color;
	size_t roundness;
}NButton;

NANIM_UIOBJ typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	Vector2 size;
	//Image imp
}NImgButton;

NANIM_UIOBJ typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	Vector2 size;
	Color color;
}NRect;

NANIM_UIOBJ typedef struct {
	void (*vtable[MAX_VTABLE_IMPL])();
	Vector2 pos;
	size_t radius;
	Color color;
}NCircle;

NANIM_TYPE typedef struct {
	Vector2 from;
	Vector2 to;
	size_t duration;
} Keyframe;

/***********************************/
void play_animation(Animation* anim);
void pause_animation(Animation* anim);

#undef NANIM_TYPE

#endif
