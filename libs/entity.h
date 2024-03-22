#ifndef ENTITY_H
#define ENTITY_H

#include "dynarr.h"
#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  eWALK,
  eIDLE,
  eJUMP,
  eCROUCH,
  eATTACK,
  eDASH,
  ePARRY,
  eDAMAGED,
  ePARRIED,
  eSTUNNED,
} StateKind;

typedef struct {
  StateKind kind;
  void *data;
} State;

typedef struct RigidBody {
  Vector2 *hitbox;
  DynArray *forces;
} RigidBody;

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
  State state;
  RigidBody *rb;
  //
  Animation *animations;
  size_t animations_length;
  size_t curr_anim_index;
} Player;

RigidBody *rb_init();
void rb_addforce(RigidBody *rb, Vector2 force);
void rb_handle_forces(RigidBody *rb);
bool has_state(Player *player, StateKind kind);
bool can_transition_state(Player *player, StateKind kind);
void add_state(Player *player);
void handle_state(Player *player);
#endif // PLAYER_H
