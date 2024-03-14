#ifndef RENDERER_H
#define RENDERER_H

#include <errno.h>
#include <pthread.h>
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef enum {
  S_WALK,
  S_IDLE,
  S_M1,
  S_BLOCK,
  S_DASH,
} StateKind;

typedef struct {
  StateKind kind;
  void *extra_data;
} State;

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
  //
  Animation *animations;
  size_t animations_length;
  size_t curr_anim_index;
} Player;

Player *player_init(Vector2 size, Color color);
void renderer_init(const int width, const int height, const char *title);
void renderer_render(const int width, const int height, Player *player);

#endif // Renderer_h
