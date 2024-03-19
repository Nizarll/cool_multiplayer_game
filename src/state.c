#include "../libs/state.h"

#define UNIMPLEMENTED
bool has_state(Player *player, StateKind kind) {
  if (!player->states)
    return false;
  for (int i = 0; i < player->states->occupied_length; i++) {
    if (((State *)player->states)[i].kind == kind)
      return true;
  }
  return false;
}
bool handle_state(Player *player) {
  // add state machine lulz
  UNIMPLEMENTED
  return false;
}
