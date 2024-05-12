#ifndef NRENDER_H
#define NRENDER_H

#include "nanim.h"
#include "utils.h"
#include <stdio.h>

#ifndef CRASH_ON_TEXTURE_FAIL
#define CRASH_ON_TEXTURE_FAIL true
#endif

typedef struct {
	const char* path;
	Texture2D sheet;
	Vector2 offset; // offset the sprite is surounded by
	Vector2 size;
	Vector2 position;
	size_t length;
	size_t counter;
}SpriteAnimation;

typedef struct {
	Animation *anims;
	SpriteAnimation *spr_anims;
	Vector2 position;
} Player;

Player* player_create(Vector2 pos,
		Animation* anims,
		SpriteAnimation* spr_anims,
		Mempool* pool);
void player_destroy(Player* player);
void spriteanim_load(SpriteAnimation* anim);
void spriteanim_update(SpriteAnimation* anim);

#endif
