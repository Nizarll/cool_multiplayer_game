#ifndef STATE_H
#define STATE_H

#include "dynarr.h"
#include "entity.h"
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

bool has_state(Player *player, StateKind kind);
bool can_transition_state(Player *player, StateKind kind);
void handle_state(Player *player);

#endif // STATE_H
