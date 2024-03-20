#include "../libs/state.h"
#include "../libs/entity.h"
#include <stdio.h>
#include <stdlib.h>

#define UNIMPLEMENTED                                                          \
  do {                                                                         \
    printf("\t\nUNIMPLEMENTED\t\n");                                           \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

bool has_state(Player *player, StateKind kind) {
  if (!player->states)
    return false;
  for (int i = 0; i < player->states->occupied_length; i++) {
    if (((State *)player->states)[i].kind == kind)
      return i;
  }
  return false;
}

// I KNOW O(N ^ ONE BILLION) is bad but whatever man im just making a 2d game
// TODO : switch to hashtable
//
bool can_transition_state(Player *player, StateKind kind) {
  switch (kind) {
  case eWALK:
    return (!has_state(player, eWALK) &&
            !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
              has_state(player, ePARRIED) || has_state(player, eSTUNNED)));
  case eJUMP:
    return (!has_state(player, eJUMP) &&
            !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
              has_state(player, ePARRIED) || has_state(player, eSTUNNED)));
  case ePARRY:
    return (!has_state(player, eJUMP) && !has_state(player, eWALK) &&
            !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
              has_state(player, ePARRIED) || has_state(player, eSTUNNED)));
  case eDASH:
    return (has_state(player, eWALK) &&
            !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
              has_state(player, ePARRIED) || has_state(player, eSTUNNED)));
  default:
    fprintf(stderr, "\tERROR: INVALID STATE TRANSITION !\t\n");
    exit(EXIT_FAILURE);
  }
  return false;
}

void handle_state(Player *player) {
  RigidBody *rb = player->rb;
  if (rb == NULL)
    return;
  if (has_state(player, eWALK) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    rb_addforce(player->rb, (Vector2){.x = .1, .y = 0});
  }
  if (has_state(player, eJUMP) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    rb_addforce(player->rb, (Vector2){.x = 0, .y = .1});
  }
  if (has_state(player, eDASH) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    rb_addforce(player->rb, (Vector2){.x = 2, .y = 0});
    // do dash
  }
}
