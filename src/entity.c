#include "../libs/entity.h"

#define DEFAULT_FORCES_LEN (5)
#define DEFAULT_FORCES_STEP (5)

RigidBody *rb_init() {
  RigidBody *rb = malloc(sizeof(RigidBody));
  rb->forces = da_init(NULL, sizeof(Vector2), DEFAULT_FORCES_LEN, 0, 5);
  rb->hitbox = NULL;
  return rb;
}
void rb_handle_forces(RigidBody *rb) {
  // compute rigidbody forces
  /*
   *
   *
   */
}
