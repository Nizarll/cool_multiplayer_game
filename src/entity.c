#include "../libs/entity.h"

#define UNIMPLEMENTED                                                          \
  do {                                                                         \
    printf("\t\nUNIMPLEMENTED\t\n");                                           \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#define DEFAULT_FORCES_LEN (5)
#define DEFAULT_FORCES_STEP (5)

RigidBody *rb_init() {
  RigidBody *rb = (RigidBody *)malloc(sizeof(RigidBody));
  rb->forces = da_init(sizeof(Vector2));
  rb->hitbox = NULL;
  return rb;
}

void rb_addforce(RigidBody *rb, Vector2 force) {
  if (rb == NULL)
    return;
  if (rb->forces == NULL)
    return;
}

bool has_state(Player *player, StateKind kind) {
  if (!player->states)
    return false;
  if (!player->states->items)
    return false;
  for (int i = 0; i < player->states->occupied_length; i++) {
    if (((State *)player->states->items)[i].kind == kind)
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
    if (((State *)player->states->items)[has_state(player, eWALK)].data) {
    }
    rb_addforce(player->rb, (Vector2){.x = .1, .y = 0});
  }
  if (has_state(player, eJUMP) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    rb_addforce(player->rb,
                (Vector2){.x = 0, .y = .1}); // 1/2 gt² + v sin a t + x
  }
  if (has_state(player, eDASH) &&
      !(has_state(player, ePARRY) || has_state(player, eDAMAGED) ||
        has_state(player, ePARRIED) || has_state(player, eSTUNNED))) {
    rb_addforce(player->rb, (Vector2){.x = 2, .y = 0});
    // do dash
  }
}
