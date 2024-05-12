#ifndef NANIM_H
#define NANIM_H

#ifndef MAX_VTABLE_IMPL
constexpr auto MAX_VTABLE_IMPL = 10;
#endif

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>

/*
 * Virtual tables allow us to implement
 * so that each rlobjecttype instance could have
 * its own implementation of the render and destroy
 * functions
 */

constexpr char error[] = "[x] ERROR ! :";

#define NANIM_ASSERT(x, msg) do {  \
	if ((!x)) { 								      \
		printf(error msg);				  		 \
		exit(EXIT_FAILURE);				     	  \
	}																		 \
}while(0)

typedef enum {
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
	void* data;
	size_t size;
} RenderItem;

typedef struct {
	RenderItem* items;
	size_t size;
}RenderBuffer;

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
	bool _internal;
} Animation;

typedef struct {
	Color color;
	bool clicked;
}Button;

/***********************************/
Animation *animation_create(EasingStyle style,
		bool loop,
		Keyframe* keyframes,
		size_t keyframes_len,
		Mempool* pool);
void renderbuffer_update(RenderBuffer* buffer);
void animation_update(Animation* anim);
void animation_play(Animation* anim);
void animation_pause(Animation* anim);


/***********************************/
#endif
