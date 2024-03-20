#include "../libs/entity.h"

#define DEFAULT_FORCES_LEN (5)
#define DEFAULT_FORCES_STEP (5)

RigidBody *rb_init() {
  RigidBody *rb = malloc(sizeof(RigidBody));
  rb->forces = da_init(NULL, sizeof(Vector2), DEFAULT_FORCES_LEN, 0, 5);
  rb->hitbox = NULL;
  return rb;
}

void rb_addforce(RigidBody *rb, Vector2 force) {
  if (rb == NULL)
    return;
  if (rb->forces == NULL)
    return;
  da_append(rb->forces, &(Vector2){
                            .x = force.x,
                            .y = force.y,
                        });
}

// void add_state(Player *player, State state) {
//   if (rb == NULL)
//     return;
//   if (rb->forces == NULL)
//     return;
//   da_append(rb->forces, &(Vector2){
//                             .x = force.x,
//                             .y = force.y,
//                         });
// }
