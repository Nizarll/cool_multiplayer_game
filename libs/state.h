#ifndef STATE_H
#define STATE_H

#include "dynarr.h"
#include <errno.h>
#include <pthread.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef enum {
  eWALK,
  eJUMP,
  eCROUCH,
  eDASH,
  ePARRY,
  eDAMAGED,
  ePARRIED,
  eSTUNNED,
} StateKind;

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

typedef struct {
  StateKind kind;
  void *data;
} State;

typedef struct Player {
  Color color;
  uint8_t rotation;
  Vector2 position;
  Vector2 size;
  State *states;
  //
  Animation *animations;
  size_t animations_length;
  size_t curr_anim_index;
} Player;

bool has_state(Player *player);
bool handle_state(Player *player);

#endif // STATE_H
