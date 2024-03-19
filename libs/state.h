#ifndef STATE_H
#define STATE_H

#include "dynarr.h"
#include "player.h"
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

bool has_state(Player *player, StateKind kind);
bool handle_state(Player *player);

#endif // STATE_H
