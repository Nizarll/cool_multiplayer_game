#include "../libs/state.h"

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

bool handle_state(Player *player) {
  // RigidBody rb = get_rigid_body(player);
  UNIMPLEMENTED;
  if (has_state(player, eWALK) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    // add_force(rb, (Vector2){.x = 10, .y = 0});
    //  do walk
  }
  if (has_state(player, eJUMP) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    // add_force(rb, (Vector2){.x = 10, .y = 0});
    //  do jump
  }
  if (has_state(player, eDASH) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    // do dash
  }
  return false;
}
