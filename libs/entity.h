#ifndef ENTITY_H
#define ENTITY_H

#include "dynarr.h"
#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Animation {
  const char *path;
  Texture2D texture;
  Rectangle frame;
  // animation variables;
  size_t length;
  size_t counter;
  size_t speed;
  size_t curr_frame;
} Animation;

typedef struct Player {
  Color color;
  uint8_t rotation;
  Vector2 position;
  Vector2 size;
  DynArray *states;
  //
  Animation *animations;
  size_t animations_length;
  size_t curr_anim_index;
} Player;

typedef struct RigidBody {
  Vector2 *hitbox;
  DynArray *forces;
} RigidBody;

RigidBody *rb_init();
RigidBody *rb_get_from_player(Player *player);
void rb_handle_forces(RigidBody *rb);
#endif // PLAYER_H
