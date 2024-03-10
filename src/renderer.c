#include "../libs/renderer.h"

#define FPS_CAP (4999)
#define PLAYER_HEIGHT (12)
#define PLAYER_WIDTH (6)

typedef enum {
  PLAYER,
  COLLIDEABLE_OBJ,
  PARTICLE,
} Kind;

typedef enum {
  IDLING,
  WALKING,
  RUNNING,
  ATTACKING,
  JUMPING,
  ATTACKED,
} State;

char *get_state_name(State state) {
  switch (state) {
  case (IDLING):
    return "IDLING";
  case (RUNNING):
    return "RUNNING";
  case (WALKING):
    return "WALKING";
  case (ATTACKING):
    return "ATTACKING";
  case (JUMPING):
    return "JUMPING";
  case (ATTACKED):
    return "ATTACKED";
  default:
    return "BRUH WTF IS THIS STATE FARK OFF";
  }
  return NULL;
}

typedef struct {
  uint16_t frame_count;
  uint16_t frame_index;
  uint16_t current_frame;
  char *name;
  char *path_to_anim;
  Rectangle frame;
} Animation;

typedef struct {
  char *name;
  State state;
  bool state_ready;
} StateSelector;

struct Object {
  const Animation *animations; // read only
  const StateSelector *states; // read only
  Animation animation;
  StateSelector state;
  StateSelector default_state;
  uint16_t animations_len;
  Kind kind;
  Vector2 position;
  Vector2 hitbox;
  uint16_t rotation;
  Color color;
  Texture2D texture;
};

static Animation set_anim(Object *object, const char *name) {
  for (int i = 0; i < object->animations_len; i++) {
    if (strcmp(object->animations[i].name, name) == 0)
      return object->animations[i];
  }
  return (Animation){};
}

static void handle_anims(Object *object) {
  if (object->kind == PLAYER) {
    if (strcmp(object->state.name, "Walking") == 0) {
      object->animation.frame_index = 0;
      object->animation.current_frame++;
      if (object->animation.current_frame > object->animation.frame_count - 1)
        object->animation.current_frame = 0;

      object->animation.frame.x = (float)object->animation.current_frame *
                                  (float)object->texture.width /
                                  object->animation.frame_count;
    } else if (strcmp(object->state.name, "Idling") == 0) {
      // play pause animation
    }
  } else if (object->kind == COLLIDEABLE_OBJ) {

  } else {
  }
}

static const StateSelector player_states[] = {
    {.name = "Walking", .state = WALKING, .state_ready = false},
    //{.name = "Walking", .state = WALKING, .state_ready = false},
    //{.name = "Walking", .state = WALKING, .state_ready = false},
};

static const StateSelector enemy_states[] = {
    {.name = "Walking", .state = WALKING, .state_ready = false},
    //{.name = "Attacking", .state = ATTACKING, .state_ready = false},
    //{.name = "Dashing", .state = DASHING, .state_ready = false},
};

/* typedef struct {
* uint16_t frame_count;
*  uint16_t frame_index;
*  uint16_t current_frame;
*  char *name;
*  char *path_to_anim;
*  Rectangle frame;
} Animation; */

static Animation *anims_init(Kind kind) {
  Animation *animations;
  switch (kind) {
  case PLAYER: {
    int i = 0;
    animations = malloc(sizeof(player_states) / sizeof(player_states[0]));
    animations[0] = (Animation){
        .frame_count = 4,
        .frame_index = 0,
        .current_frame = 0,
        .name = "Walking",
        .path_to_anim = "./resources/character_run_cycle.png",
        .frame =
            (Rectangle){

            },
    };
  }
    return animations;
  case PARTICLE:

    return;
  case COLLIDEABLE_OBJ:
    return;
  }
  return NULL;
}

static void destroy_obj(Object *obj) { free(obj->animations); }

static Object obj_init(Kind kind, Vector2 position, uint16_t size) {
  Object obj = {0};
  switch (kind) {
  case PLAYER:
    return obj = (Object){
               .hitbox = (Vector2){.x = PLAYER_WIDTH, .y = PLAYER_HEIGHT},
               .position =
                   (Vector2){
                       .x = position.x / 2.0,
                       .y = position.y / 2.0,
                   },
               .animations = anims_init(kind),
               .states = player_states,
           };
  case PARTICLE:
    return obj = (Object){
               .hitbox = (Vector2){.x = PLAYER_WIDTH, .y = PLAYER_HEIGHT},
               .position =
                   (Vector2){
                       .x = position.x / 2.0,
                       .y = position.y / 2.0,
                   },
               //    .texture_path = "./resources/character_run_cycle.png",
               //    .texture = LoadTexture(obj.texture_path),
               //    .texture.width = obj.texture.width * size,
               //    .texture.height = obj.texture.height * size,
               //    .states = player_states,
           };
  case COLLIDEABLE_OBJ:
    return obj = (Object){
               .hitbox = (Vector2){.x = PLAYER_WIDTH, .y = PLAYER_HEIGHT},
               .position =
                   (Vector2){
                       .x = position.x / 2.0,
                       .y = position.y / 2.0,
                   },
               //    .texture_path = "./resources/character_run_cycle.png",
               //    .texture = LoadTexture(obj.texture_path),
               //    .states = player_states,
           };
  }
  return obj;
}

void renderer_init(const int width, const int height, const char *title) {
  InitWindow(width, height, title);
  SetTargetFPS(FPS_CAP);
  Object player = obj_init(
      PLAYER, (Vector2){.x = (float)width / 2, .y = (float)height / 2}, 5);
  Rectangle frame_rec = {0.0f, 0.0f, (float)player.texture.width / 4,
                         (float)player.texture.height};
  int frames_counter = 0, current_frame = 0;
  int frames_speed = 1;
  while (!WindowShouldClose()) {
    frames_counter++;
    if (frames_counter >= (int)(60 / .15)) {
      frames_counter = 0;
      current_frame++;

      if (current_frame > 3)
        current_frame = 0;

      frame_rec.x = (float)current_frame * (float)player.texture.width / 4;
    }
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    DrawTextureRec(player.texture, frame_rec,
                   (Vector2){
                       .x = player.texture.width / 2.0,
                       .y = player.texture.height / 2.0,
                   },
                   WHITE);
    EndDrawing();
  }
  CloseWindow();
}
