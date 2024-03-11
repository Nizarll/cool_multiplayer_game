#include "../libs/renderer.h"
#include <raylib.h>
#include <stdint.h>

#define FPS_CAP (4999)
#define PLAYER_HEIGHT (12)
#define PLAYER_WIDTH (6)

#define OPT(T) (option(T))

float option(float t) {
  if (!t)
    return 1;
  return t;
}

typedef struct {
  uint16_t keys_pressed;
} InputTracker;

typedef struct {
  Rectangle frame;
  char *path;
  Texture2D sprite_sheet;
  uint16_t length;
  uint16_t frames_counter;
  uint16_t current_frame;
} Animation;

typedef struct {
  Animation *animations;
  uint16_t current_anim_index;
  uint16_t length;
} AnimationGroup;

typedef struct {
  Vector2 position;
  Vector2 size;
  AnimationGroup anim_group;
  InputTracker *input_tracker;
} Entity;

static const Animation idle_anim = (Animation){
    .path = "./resources/character.png",
    .length = 1,
};
static const Animation walk_anim = (Animation){
    .path = "./resources/character_walk_cycle.png",
    .length = 4,
};
static const Animation run_anim = {0};
static const Animation hit_anim = {0};

static Animation player_animations[] = {idle_anim, run_anim, walk_anim,
                                        hit_anim};

static Entity player = (Entity){
    .size = (Vector2){.x = 5.0, .y = 5.0},
    .position = (Vector2){.x = 300.0f, .y = 250.0f},
    .anim_group = (AnimationGroup){
        .animations = player_animations,
        .length = sizeof(player_animations) / sizeof(player_animations[0]),
        .current_anim_index = 0,
    }};

void handle_input(Entity *entity) {
  if (entity->input_tracker == NULL)
    return;
  InputTracker *input_tracker = entity->input_tracker;
  if (IsKeyDown(KEY_A)) {
    input_tracker->keys_pressed += 1;
  }
  if (IsKeyDown(KEY_D)) {
    input_tracker->keys_pressed += 1;
  }
}

//  Rectangle frame;
//  char *path;
//  Texture2D sprite_sheet;
//  uint16_t length;
//  uint16_t frames_counter;
//  uint16_t current_frame;
//} Animation;

void init_entity(Entity *entity) {
  if (entity->anim_group.length <= 0)
    return;
  for (int i = 0; i < entity->anim_group.length; i++) {
    if (!(entity->anim_group.animations[i].path))
      continue;
    entity->anim_group.animations[i].frames_counter = 0;
    entity->anim_group.animations[i].current_frame = 0;
    entity->anim_group.animations[i].sprite_sheet =
        LoadTexture(entity->anim_group.animations[i].path);
    entity->anim_group.animations[i].frame = (Rectangle){
        .x = .0f,
        .y = .0f,
        .width = (float)entity->anim_group.animations[i].sprite_sheet.width /
                 entity->anim_group.animations[i].length * OPT(entity->size.x),
        .height = (float)entity->anim_group.animations[i].sprite_sheet.height /
                  entity->anim_group.animations[i].length * OPT(entity->size.y),
    };
  }
}
void destroy_entity(Entity *entity) {
  free(entity->input_tracker);
  free(entity->anim_group.animations);
  for (int i = 0; i < entity->anim_group.length; i++) {
    if (entity->anim_group.animations[i].path)
      free((void *)entity->anim_group.animations[i].path);
  }
}

void handle_anims(Entity *entity) {
  Animation curr_anim =
      entity->anim_group.animations[entity->anim_group.current_anim_index];
  curr_anim.frames_counter++;
  if (curr_anim.frames_counter >= (60 / .2)) { // change later
    curr_anim.frames_counter = 0;
    curr_anim.current_frame++;
    if (curr_anim.current_frame > curr_anim.length - 1)
      curr_anim.current_frame = 0;
    curr_anim.frame.x = (float)curr_anim.current_frame *
                        (float)curr_anim.sprite_sheet.width / curr_anim.length;
  }
}
void handle_movement(Entity *entity) {
  if (IsKeyDown(KEY_A)) {
    entity->position = Vector2Add(
        entity->position,
        (Vector2){.x = -(float)1 / entity->input_tracker->keys_pressed,
                  .y = 0});
  }
  if (IsKeyDown(KEY_D)) {
    entity->position = Vector2Add(
        entity->position,
        (Vector2){.x = (float)1 / entity->input_tracker->keys_pressed, .y = 0});
  }
}

void renderer_init(const int width, const int height, const char *title) {
  InitWindow(width, height, title);
  SetTargetFPS(FPS_CAP);
  init_entity(&player);

  player.anim_group.current_anim_index = 1;

  while (!WindowShouldClose()) {
    handle_input(&player);
    handle_movement(&player);
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    handle_anims(&player);
    DrawTextureRec(
        player.anim_group.animations[player.anim_group.current_anim_index]
            .sprite_sheet,
        player.anim_group.animations[player.anim_group.current_anim_index]
            .frame,
        (Vector2){
            .x = 300.0f,
            .y = 250.0f,
        },
        WHITE);
    EndDrawing();
  }
  CloseWindow();
  destroy_entity(&player);
}
