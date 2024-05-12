#include "../libs/nrender.h"

#define screenWidth 400
#define screenHeight 400

void spriteanim_load(SpriteAnimation* anim) {
	if(anim->path == NULL){
		printf("error ! :invalid sprite animation texture path\n");
		exit(EXIT_FAILURE);
	}
	anim->sheet = LoadTexture(anim->path);
	if(anim->sheet.id <= 0) {
		printf("error ! :sprite sheet loading texture failed! error code: %d\n", anim->sheet.id);
		if (CRASH_ON_TEXTURE_FAIL)
			exit(EXIT_FAILURE);
	}
}

void spriteanim_update(SpriteAnimation* anim) {
	if (anim->counter < anim->length * 10)
		anim->counter += 1;
	else anim->counter = 0;
	float frame_x;
	float frame_y;
	frame_y = anim->offset.y / 2;
	if (anim->counter == 0) {
		frame_x = anim->offset.x / 2;
	} else {
		frame_x = (anim->counter / 10) * (anim->offset.x + 16.0f) + 32.0f;
	}
//	printf("Index is : %d, frame x is : %.2f frame y is : %.2f \n",anim->counter, frame_x, frame_y);
	//DrawTexturePro(anim->sheet, (Rectangle){(float)frame_x, (float)frame_y, 16.0f, 16.0f }, 
   //     (Rectangle) { screenWidth / 2.0f, screenHeight / 2.0f, 200.0f, 200.0f }, (Vector2){8.0f, 8.0f}, 0.0f, WHITE);
}

Player* player_create(Vector2 pos,Animation* restrict anims, SpriteAnimation* restrict spr_anims, Mempool* restrict pool) {
	Player* p;
	if (pool)
		p = malloc(sizeof(Player));
	else p = mempool_alloc(pool, sizeof(Player));
	if(!p) {
		printf("error !: could not create player object !");
		exit(EXIT_FAILURE);
	}
	p->position = pos;
	p->spr_anims = spr_anims;
	p->anims = anims;
	return p;
}
void player_destroy(Player* player) {
	free((void*) player);
}
